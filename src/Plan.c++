#include "Plan.h++"

#include "ExtractionMethods.h++"
#include "Keyword.h++"
#include "Reference.h++"
#include "Registry.h++"

#include <istream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::istream &Plan::extract ( std::istream &istream ) {
    std::stringstream line;
    std::string temp;
    // plans begin with "plan"
    if ( !extractToKeyword ( istream , "plan" ) ) {
        return istream;
    }
    // now we only have a few different possible keywords:
    // 1. ref - the reference the plan uses
    // 2. semester {reference} {list of courses} - a semester that uses reference and has these courses specified
    // 3. endplan - end
    auto convert = [ ] ( Serial &plan ) -> Plan &{
        return ( Plan & ) plan;
    };
    auto referenceFunction = [ & ] ( std::stringstream &is , Serial &p ) {
        convert ( p ).grabReference ( is );
    };
    auto semesterFunction = [ & ] ( std::stringstream &is , Serial &p ) {
        Reference semester;
        is >> semester;
        while ( !is.eof ( ) ) {
            Reference course;
            is >> course;
            convert ( p ).semesters [ semester ].push_back ( course ); 
        }
    };

    static Keyword keywords [ ] = {
        Keyword ( "ref" , std::function ( referenceFunction ) ) ,
        Keyword ( "semester" , std::function ( semesterFunction ) ),
    };

    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
        if ( isComment ( temp ) ) continue;
        else for ( auto i = 0LLU; i < sizeof ( keywords ) / sizeof ( keywords [ 0 ] ); i++ ) {
            if ( keywords [ i ].keyword == temp ) {
                keywords [ i ].function ( line , ( Referred & ) *this );
            }
        }
    } while ( temp != "endplan" );
    return istream;
}

std::string const generateError ( Plan const &plan , Registry const &registry , Reference const &course , Reference const &requisites ) {
    std::string message = plan.getReference ( ).getName ( );
    Course *pCourse = registry.resolveCourse ( course );
    if ( !pCourse ) {
        message += " fails because " + course.getName ( ) + " cannot be resolved. Is it in the data files?";
    } else {
        message += " fails because " + course.getName ( ) + " does not meet the requisite " + requisites.getName ( );
    }
    return message;
}

std::string const Plan::getPlanMessage ( Registry const &registry ) const noexcept {
    // all of our semesters in chronological (loaded) order
    // because of how std::map works, there is no guarantee that we would iterate through
    // the memebers in order (std::map uses a self-balancing BST, and that would only be in order
    // if the iterator uses an inorder-iteration).
    std::vector < Reference > orderedSemesters = registry.semestersInOrder ( );
    // All of the courses that we have looked at and already verified along with the ones that
    // we are verifying.
    std::vector < std::vector < Reference > > currentAndPriorSemesters;
    // The reference that any non-fufilled course tells us is not met. This reference
    // is to a Requisites.
    Reference firstOffendingRequisiteGroup;
    for ( auto i = 0LLU; i < orderedSemesters.size ( ); i++ ) {
        // If our plan has this semester; plans do not necessarily have to have a semester
        // which has already been defined.
        if ( semesters.contains ( orderedSemesters [ i ] ) ) {
            // This semester is now the one we are verifying.
            currentAndPriorSemesters.push_back ( semesters.at ( orderedSemesters [ i ] ) );
            // only check the prerequisites if we should check the semester. No check for 
            // nullpointer is made since only defined semesters would show up in the registry
            if ( registry.resolveSemester ( orderedSemesters [ i ] )->isChecked ( ) ) {
                // for each course *we* have in this semester
                for ( Reference course : semesters.at ( orderedSemesters [ i ] ) ) {
                    // resolve the course, if we can
                    Course *pcourse = registry.resolveCourse ( course );
                    if ( !pcourse ) { // if the course cannot be resolved, we take it as an error
                        return generateError ( *this , registry , course , firstOffendingRequisiteGroup );
                    } else {
                        // if the course has unmet requisites, that is an error.
                        if ( !pcourse->meetsRequisites ( currentAndPriorSemesters , registry , firstOffendingRequisiteGroup ) ) {
                            return generateError ( *this , registry , course , firstOffendingRequisiteGroup );
                        }
                    }
                }
            }
        }
    }
    return getReference ( ).getName ( ) + " passes.";
}