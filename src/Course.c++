#include "Course.h++"

#include "ExtractionMethods.h++"
#include "Keyword.h++"
#include "Registry.h++"

#include <functional>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>


std::istream &Course::extract ( std::istream &istream ) {

    auto convert = [ ] ( Serial &course ) -> Course & {
        return (Course &) ( Referred & ) course;
    };

    auto appendFunction = [ ] ( std::string &variable , std::string const keyword , std::stringstream &sstream ) {
        variable += sstream.str ( ).substr ( sstream.str ( ).find ( keyword ) + keyword.size ( ) + 1 );
    };

    auto nameFunction = [ & ] ( std::stringstream &is , Serial &c ) {
        appendFunction ( convert ( c ).name , std::string ( "name" ) , is );
    };
    auto descriptionFunction = [ & ] ( std::stringstream &is , Serial &c ) {
        appendFunction ( convert ( c ).desc , std::string ( "desc" ) , is );
    };

    auto requisitesFunction = [ & ] ( std::stringstream &is , Serial &c ) {
        Reference reference;
        is >> reference;
        convert ( c ).requisites.push_back ( reference );
    };

    auto hoursFunction = [ & ] ( std::stringstream &is , Serial &c ) {
        is >> convert ( c ).hours;
    };

    static Keyword keywords [ ] = {
        Keyword ( "ref" , wrap ( referenceFunction ) ) ,
        Keyword ( "name" ,std::function ( nameFunction ) ) ,
        Keyword ( "desc" ,std::function ( descriptionFunction ) ) ,
        Keyword ( "reqs" ,std::function ( requisitesFunction ) ) ,
        Keyword ( "hours" ,std::function ( hoursFunction ) ) ,
    };


    std::stringstream line;
    std::string temp;
    // the course keyword begins with the keyword "course"
    // so iterate until we find that
    if ( !extractToKeyword ( istream , "course" ) ) {
        return istream;
    }
    // now we iterate with only a few different possible keywords:
    // we can have the course reference, "ref"
    // we can have the course name (or part of it), "name"
    // we can have the course description (or part of it), "desc"
    // we can have the course requisites (or some of them), "reqs"
    // we can have a flag (does not match any keyword)
    // we can have the hours, "hours", followed by a double value.
    // or we can find "endcourse" and be done
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
        //std::cout << "Keyword: " << temp << std::endl;
        if ( isComment ( temp ) ) continue;
        else if ( temp == "endcourse" ) {
            break;
        } else {
            bool matched = false;
            for ( auto i = 0LLU; i < sizeof ( keywords ) / sizeof ( keywords [ 0 ] ); i++ ) {
                if ( keywords [ i ].keyword == temp ) {
                    keywords [ i ].function ( line , ( Referred & ) *this );
                    matched = true;
                }
            }
            if ( !matched ) addFlag ( temp );
        }
    } while ( true );
    return istream;
}

std::vector < Requisites *> const Course::resolveRequisites ( Registry const &withRegistry ) noexcept {
    std::vector <Requisites *> output;
    for ( Reference requisites : this->requisites ) {
        output.push_back ( withRegistry.resolveRequisites ( requisites ) );
    }
    return output;
}

bool const Course::meetsRequisites ( std::vector < std::vector < Reference > > const &courses , Registry const &registry , Reference &offending ) {
    // this vector is orientated in semesters going from beginning (courses[0]) to end (courses.back())

    // if there are no courses, we do not meet the prerequisites
    for ( Reference requisiteGroup : requisites ) {
        Requisites *prequisites = registry.resolveRequisites ( requisiteGroup );
        if ( prequisites ) {
            bool foundMatch = false;

            for ( auto semester : courses ) {
                for ( Reference course : semester ) {
                    Course *pcourse = registry.resolveCourse ( course );
                    if ( pcourse ) {
                        bool matchState = prequisites->meetsRequisite ( *pcourse );
                        if ( matchState ) {
                            std::cout << course << " matches " << getReference ( ) << "\n";
                        }
                        foundMatch |= matchState;
                    }
                }
            }
            offending = requisiteGroup;
            if ( !foundMatch ) return false;
        }
    }

    return true; // if we are here, either all requisites were met, or there are no requisites
}