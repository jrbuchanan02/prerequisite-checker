/**
 * \file Requisite.c++
 * \brief defines the extraction methods for Requisite and IndividualRequisite
 * \author Joshua Buchanan
 * \copyright Joshua Buchanan (C) 2021; MIT License
 * \date 2021-10-31 (Happy Halloween)
 */
#include "Requisite.h++"

#include "CourseReference.h++"

#include <istream>
#include <sstream>
#include <string>
#include <vector>

constexpr char const *const requisiteBegin = "req";
constexpr char const *const allowConcurrent = "con";
constexpr char const *const allowPreviously = "pre";


std::istream &operator >> ( std::istream &istream , IndividualRequisite &individualRequisite ) {
    // we expect this format:
    // pre|con [pre|con] SUBJECT NUMBER ... endl
    // where ... is any number of other IndividualRequisites
    // therefore, we know that we will take at least 3 words
    // we also know that our second to last item will be a string and
    // will not be the allowConcurrent nor allowPreviously keyword
    std::string temp;
    for ( istream >> temp; temp == allowConcurrent || temp == allowPreviously; istream >> temp ) {
        if ( temp == allowConcurrent ) individualRequisite.concurrent = true;
        if ( temp == allowPreviously ) individualRequisite.previously = true;
    }
    // now, we have a temp that holds the subject
    std::uintmax_t courseNumber;
    istream >> courseNumber;
    individualRequisite.referredCourse = CourseReference ( temp , courseNumber );
    return istream;
}

std::istream &operator >> ( std::istream &istream , Requisite &requisite ) {
    // we expect this format:
    // req individualRequisite ... endl
    // where the ... represents any number of individual requisites
    // we know that the line *must* begin with the requisiteBegin keyword, so we should
    // just keep getting the line until we find it
    std::stringstream line;
    std::string temp;
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
    } while ( temp != requisiteBegin );
    while (!line.eof ( ) ) {
        IndividualRequisite individualRequisite(line);
        requisite.requisites.push_back ( individualRequisite );
    }
    return istream;
}