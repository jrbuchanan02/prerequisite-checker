/**
 * \file Course.c++
 * \brief Defines the extraction method for Course
 * \author Joshua Buchanan
 * \copyright Joshua Buchanan (C) 2021; MIT License
 * \date 2021-10-31 (Happy Halloween!)
 */

#include "Course.h++"

#include "CourseReference.h++"
#include "Requisite.h++"

#include <istream>
#include <sstream>
#include <string>
#include <vector>

constexpr char const *const courseBegin = "course";
constexpr char const *const reference = "ref";
constexpr char const *const name = "name";
constexpr char const *const desc = "desc";
constexpr char const *const hours = "hours";
constexpr char const *const honors = "honors";
constexpr char const *const graduate = "graduate";
constexpr char const *const requisitesBegin = "reqs";
constexpr char const *const requisitesEnd = "endreqs";
constexpr char const *const courseEnd = "endcourse";

std::istream &operator >> ( std::istream &istream , Course &course ) {
    // the format we are expecting
    // course
    //      ref {CourseReference}
    //      name {Some Human Readable Name, ex. CS II for Majors}
    //      ...1
    //      desc {Some Human Readable Description, ex. In this class students will learn the concepts of Calculus in multiple variables}
    //      ...2
    //      hours {number -- that we can extract a double from}
    //      [honors]
    //      [graduate]
    //      reqs
    //          req ...
    //          ...3
    //      endreqs
    // endcourse
    // Where ...1 is any number of name keys, ...2 is any number of desc keys, and ...3 is any 
    // number of req ... keys. 
    // Note: keys may appear in any order
    // we know that the course can only begin with the courseBegin keyword so we should
    // just keep getting the line until we find it
    std::stringstream line;
    std::string temp;
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
    } while ( temp != courseBegin );
    // we know that the course can only end with the endcourse keyword, so just keep finding 
    // keywords and getting the line until we find it
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
        // check for the keywords in the order they are declared in Course
        if (temp == reference) {
            line >> course.courseReference;
        } else if  (temp == requisitesBegin) do {
            // while we cannot find the requisitesEnd keyword, 
            // keep adding requisites
            std::getline ( istream , temp );
            line = std::stringstream ( temp );
            
            if ( temp.find ( requisitesEnd ) != std::string::npos ) break;

            course.requisites.push_back ( Requisite ( line ) );
        } while (true);
        else if (temp == name ) {
            if ( course.name != "" ) course.name += " ";
            course.name += line.str ( );
        } else if ( temp == desc ) {
            if (course.desc != "" ) course.desc += " ";
            course.desc += line.str ( );
        } else if (temp == honors ) {
            course.honors = true;
        } else if (temp == graduate ) {
            course.graduate = true;
        } else if ( temp == hours ) {
            line >> course.hours;
        }
    } while (temp != courseEnd );
    return istream;
}