#include "Course.h++"

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
    do {
        std::getline ( istream , temp , '\xa' );
        line = std::stringstream ( temp );
        //std::cout << line.str ( ) << "\n";
        line >> temp;
        //std::cout << "\"" << temp << "\"\n";
#ifdef __POSIX__ // on WSL2, opening the courses file goes horribly wrong..
        std::cin.get ( );
#endif
    } while ( temp != "course" );
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

bool const Course::meetsRequisites ( std::vector < std::vector < Reference > > const &courses , Registry const &registry ) {
    // this vector is orientated in semesters going from beginning (courses[0]) to end (courses.back())

    // if there are no courses, we do not meet the prerequisites
    for ( Reference requisiteGroup : requisites ) {
        Requisites *prequisite = registry.resolveRequisites ( requisiteGroup );
        bool met = false;
        if (prequisite) {
            for ( auto semester : courses ) {
                for ( Reference course : semester ) {
                    Course *pcourse = registry.resolveCourse ( course );
                    // if we can find the course, then check if it meets the
                    // requisite
                    if ( pcourse ) {
                        met = prequisite->meetsRequisite ( *pcourse );
                    }
                    // if we cannot find the course, then it does not meet the
                    // requisite.
                }
            }
        } else {
            // TODO: insert some error condition
        }
        if (!met) return false;
    }
    return true;
}