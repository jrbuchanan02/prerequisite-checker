#include "Course.h++"

#include "ExtractionMethods.h++"
#include "Registry.h++"

#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

std::istream &Course::extract ( std::istream &istream ) {
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
        if ( temp == "endcourse" ) {
            break;
        } else if ( temp == "ref" ) {
            grabReference ( line );
        } else if ( temp == "name" ) {
            if ( name != "" ) name += " ";
            name += line.str ( ).substr ( line.str ( ).find ( temp ) + temp.size ( ) + 1 );
        } else if ( temp == "desc" ) {
            if ( desc != "" ) desc += " ";
            desc += line.str ( ).substr ( line.str ( ).find ( temp ) + temp.size ( ) + 1 );
        } else if ( temp == "reqs" ) {
            // grab the reference
            Reference found;
            line >> found;
            requisites.push_back ( found );
        } else if ( temp == "hours" ) {
            line >> hours;
        } else {
            addFlag ( temp );
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
            if (!foundMatch) return false;
        }
    }

    return true; // if we are here, either all requisites were met, or there are no requisites
}