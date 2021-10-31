/**
 * \file Semester.h++
 * \brief Defines what a Semester is
 * \author Joshua Buchanan
 * \copyright Joshua Buchanan (C) 2021; MIT License
 * \date 2021-10-31 (Happy Halloween!)
 */
#pragma once

#include "CourseReference.h++"

#include <istream>
#include <string>
#include <vector>

 /**
  * \class Semester
  * \brief Defines a Semester.
  * \note Semesters are also used to describe incoming credit. For example, incoming AP credit may be defined
  * as a semester referred to as "AP"
  * \author Joshua Buchanan
  */
class Semester {
private:
    std::string name;
    bool check = true;
    std::vector < CourseReference > courseReferences;
public:
    Semester ( ) noexcept = default;
    Semester ( Semester const & ) noexcept = default;
    Semester ( std::istream &istream ) { istream >> *this; }
    Semester ( std::string const &name , bool const &check ,
        std::vector < CourseReference > const &courseReferences ) noexcept :
        name ( name ) , check ( check ) ,
        courseReferences ( courseReferences ) { }
    ~Semester ( ) = default;

    std::string const &getName ( ) const noexcept { return name; }
    bool const &checksRequisites ( ) const noexcept { return check; }
    std::vector < CourseReference > const &getCourseReferences ( ) const noexcept { return courseReferences; }

    Semester &operator = ( Semester const & ) noexcept = default;

    friend std::istream &operator >> ( std::istream & , Semester & );
};