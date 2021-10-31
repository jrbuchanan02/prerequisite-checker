/**
 * \file Semester.c++
 * \brief Implements the extraction function for Semester
 * \author Joshua Buchanan
 * \copyright Joshua Buchanan (C) 2021; MIT License
 * \date 2021-10-31 (Happy Halloween!)
 */
#include "Semester.h++"

#include "CourseReference.h++"

#include <istream>
#include <sstream>
#include <string>
#include <vector>

constexpr char const *const semesterBegin = "semester";
constexpr char const *const name = "name";
constexpr char const *const nocheck = "nocheck";
constexpr char const *const take = "take";
constexpr char const *const semesterEnd = "endsemester";

std::istream &operator >> ( std::istream &istream , Semester &semester ) {
    // this is the format we are expecting:
    // semester
    //      name {name}
    //      [nocheck]
    //      take {course references...}
    //      ... 
    // endsemester
    // Where the ... can represent any number of take-keywords
    // we know that we only begin a semester on the semesterBegin keyword, so we should 
    // just keep going until then...
    std::stringstream line;
    std::string temp;
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
    } while ( temp != semesterBegin );
    // we know that the semester ends with the endsemester keyword, so just keep adding 
    // keywords until we find it.
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
        if ( temp == name ) {
            // like with Course, we can allow multiple lines if we want a long name
            if ( semester.name != "" ) semester.name += " ";
            semester.name += line.str ( );
        } else if ( temp == nocheck ) {
            semester.check = false;
        } else if ( temp == take ) {
            // add the entire line of course references to the semester
            while ( !line.eof ( ) ) {
                semester.courseReferences.push_back ( CourseReference ( line ) );
            }
        }
    } while ( temp != semesterEnd );
    return istream;
}