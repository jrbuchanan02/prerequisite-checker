/**
 * \file Course.h++
 * \brief Defines the Course class
 * \author Joshua Buchanan
 * \copyright Joshua Buchanan (C) 2021; MIT License
 * \date 2021-10-31 (Happy Halloween!)
 */
#pragma once

#include "CourseReference.h++"
#include "Requisite.h++"

#include <istream>
#include <string>
#include <vector>


 /**
  * \class Course
  * \brief A course within a Catalog. A Course has a certain number of hours which may be
  * honors and / or graduate. A Course also has a reference (in the form of a CourseReference)
  * that it can be referred by; similarly, it also has a name (ex, CS II for Majors) and a
  * description (ex, Students will learn ... about ... ).
  */
class Course {
private:
    // any of these requisites must be met for the course to be allowed
    CourseReference courseReference;
    std::vector < Requisite > requisites;
    std::string name;
    std::string desc;
    bool honors , graduate;
    double hours; // some courses may be a fractional number of hours
public:
    Course ( ) noexcept = default;
    Course ( Course const & ) noexcept = default;
    Course ( std::istream &istream ) { istream >> *this; }
    /**
     * TODO: implement a Course constructor that uses the named Container requirement and
     * limits Container to being a Container of Requisite.
     */
    Course ( CourseReference const &courseReference ,
        std::vector < Requisite > const &requisites , std::string const &name ,
        std::string const &desc , bool const &honors , bool const &graduate ,
        double const &hours ) noexcept :
        courseReference ( courseReference ) , requisites ( requisites ) ,
        name ( name ) , desc ( desc ) , honors ( honors ) , graduate ( graduate ) ,
        hours ( hours ) { }
    ~Course ( ) = default;

    CourseReference const &getCourseReference ( ) const noexcept { return courseReference; }
    std::vector < Requisite > const &getRequisites ( ) const noexcept { return requisites; }
    std::string const &getName ( ) const noexcept { return name; }
    std::string const &getDesc ( ) const noexcept { return desc; }
    bool const &isHonors ( ) const noexcept { return honors; }
    bool const &isGraduate ( ) const noexcept { return graduate; }
    double const &getHours ( ) const noexcept { return hours; }

    Course &operator = (Course const &) noexcept = default;
    friend std::istream &operator >> ( std::istream & , Course & );
};