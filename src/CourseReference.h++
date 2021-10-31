/**
 * \file CourseReference.h++
 * \brief defines a course reference
 * \author Joshua Buchanan
 * \copyright Joshua Buchanan (C) 2021; MIT License
 * \date 2021-10-31 (Happy Halloween)
 */
#pragma once

#include <cstdint>
#include <sstream>
#include <string>

class CourseReference {
private:
    std::string subject;
    std::uintmax_t number;
public:
    CourseReference ( ) noexcept = default;
    CourseReference ( std::string const &subject , std::uintmax_t const &number ) noexcept :
        subject ( subject ) , number ( number ) {}
    CourseReference ( CourseReference const & ) noexcept = default;
    CourseReference ( std::istream &istream ) { istream >> *this; }
    ~CourseReference ( ) = default;

    std::string const &getSubject ( ) const noexcept { return subject; }
    std::uintmax_t const &getNumber ( ) const noexcept { return number; }
    
    CourseReference &operator = ( CourseReference const & ) noexcept = default;

    friend std::istream &operator >> ( std::istream &istream , CourseReference &courseReference ) {
        istream >> courseReference.subject;
        std::string number;
        istream >> number;
        std::stringstream(number) >> courseReference.number;
        return istream;
    }
};