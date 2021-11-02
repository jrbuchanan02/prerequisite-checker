#include "Plan.h++"

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
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
    } while ( temp != "plan" );
    // now we only have a few different possible keywords:
    // 1. ref - the reference the plan uses
    // 2. semester {reference} {list of courses} - a semester that uses reference and has these courses specified
    // 3. endplan - end
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
        if ( temp == "ref" ) {
            grabReference ( line );
        } else if ( temp == "semester" ) {
            Reference semester;
            line >> semester;
            while ( !line.eof ( ) ) {
                Reference course;
                line >> course;
                semesters [ semester ].push_back ( course );
            }
        }
    } while ( temp != "endplan" );
    return istream;
}

std::string const Plan::getPlanMessage ( Registry const &registry ) const noexcept {
    std::vector < Reference > orderedSemesters = registry.semestersInOrder ( );
    std::vector < std::vector < Reference > > currentAndPriorSemesters;
    for ( auto i = 0LLU; i < orderedSemesters.size ( ); i++ ) {
        if ( semesters.contains ( orderedSemesters [ i ] ) ) {
            currentAndPriorSemesters.push_back ( semesters.at ( orderedSemesters [ i ] ) );
            if ( registry.resolveSemester ( orderedSemesters [ i ] )->isChecked ( ) ) {
                for ( Reference course : semesters.at ( orderedSemesters [ i ] ) ) {
                    Course *pcourse = registry.resolveCourse ( course );
                    if ( !pcourse ) {
                        return getReference ( ).getName ( ) + " fails becasue " + course.getName ( ) + " cannot be resolved.";
                    } else {
                        if ( !pcourse->meetsRequisites ( currentAndPriorSemesters , registry ) ) {
                            return getReference ( ).getName ( ) + " fails because " + course.getName ( ) + " has unmet prerequisites.";
                        }
                    }
                }
            }
        }
    }
    return getReference ( ).getName ( ) + " passes.";
}