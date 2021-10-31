/**
 * \file Requisite.h++
 * \brief defines the Requisite class
 * \note In order for a requisite to be satisfied, it must have all of its courses contained in some Plan's previous Semester or that
 * Semester must not have its Requisites checked.
 * \author Joshua Buchanan
 * \copyright Joshua Buchanan (C) 2021; MIT License
 * \date 2021-10-31 (Happy Halloween)
 */
#pragma once

#include "CourseReference.h++"

#include <istream>
#include <string>
#include <vector>

/**
 * \class Individual Requisite
 * \brief a singular requisite. Can be taken concurrently with, previous to, or both concurrently / prior to the course.
 * \author Joshua Buchanan
 */
class IndividualRequisite {
private:
    // whether to allow the two courses to be taken concurrently (this means to also check in the tested semester)
    bool concurrent = false;
    // whether to allow the the required course to be taken prior to the tested course (this means to check all previous semesters)
    bool previously = false;
    // the course to refer to
    CourseReference referredCourse;
public:
    IndividualRequisite ( ) noexcept = default;
    IndividualRequisite ( bool const &c , bool const &p , CourseReference const &r ) noexcept :
        concurrent ( c ) , previously ( p ) , referredCourse ( r ) { }
    IndividualRequisite ( IndividualRequisite const & ) noexcept = default;
    IndividualRequisite ( std::istream &istream ) { istream >> *this; }
    ~IndividualRequisite ( ) = default;

    bool const &allowConcurrent ( ) const noexcept { return concurrent; }
    bool const &allowPreviously ( ) const noexcept { return previously; }
    CourseReference const &getReferredCourse ( ) const noexcept { return referredCourse; }

    IndividualRequisite &operator = ( IndividualRequisite const &) noexcept = default;

    friend std::istream &operator >> ( std::istream & , IndividualRequisite & );
};

/**
 * \class Requisite
 * \brief A Requisite, contains one or more IndividualRequisites, all IndividualRequisites must be satisfied for the
 * requisite to be satisfied
 * \author Joshua Buchanan
 */
class Requisite {
private:
    // the individual requisites available
    std::vector < IndividualRequisite > requisites;
public:
    Requisite ( ) noexcept = default;
    Requisite ( std::vector < IndividualRequisite > const &container ) {
        for ( IndividualRequisite individualRequisite : container ) {
            requisites.push_back ( individualRequisite);
        }
    }
    Requisite ( Requisite const & ) noexcept = default;
    Requisite ( std::istream &istream ) { istream >> *this; }
    ~Requisite ( ) = default;

    std::vector < IndividualRequisite > const &getRequisites ( ) const noexcept { return requisites; }

    friend std::istream &operator >> ( std::istream & , Requisite & );
};