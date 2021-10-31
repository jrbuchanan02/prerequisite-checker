/**
 * \file Plan.h++
 * \brief Defines what a Plan is
 * \author Joshua Buchanan
 * \copyright Joshua Buchanan (C) 2021; MIT License
 * \date 2021-10-31 (Happy Halloween!)
 */
#pragma once

#include "Semester.h++"

#include <istream>
#include <map>
#include <string>

/**
 * \class Plan
 * \brief a plan for all semesters in College.
 * \author Joshua Buchanan
 */
class Plan {
private:
    std::map < std::string , Semester > semesters;
    std::string planName;
public:
    Plan ( ) noexcept = default;
    Plan ( Plan const & ) noexcept = default;
    Plan ( std::istream &istream ) { istream >> *this; }
    Plan ( std::map < std::string , Semester > semesters , std::string planName ) noexcept :
        semesters ( semesters ) , planName ( planName ) { }
    ~Plan ( ) = default;

    std::string const &getName ( ) const noexcept { return planName; }
    std::map < std::string , Semester > const &getSemesters ( ) const noexcept { return semesters; }

    Plan &operator = (Plan const & ) noexcept = default;

    friend std::istream &operator >> ( std::istream & , Plan &);
};