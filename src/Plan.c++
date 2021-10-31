/**
 * \file Plan.c++
 * \brief Defines the extraction function for Plan
 * \author Joshua Buchanan
 * \copyright Joshua Buchanan (C) 2021; MIT License
 * \date 2021-10-31 (Happy Halloween!)
 */
#include "Plan.h++"

#include "Semester.h++"

#include <istream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::istream &operator >> ( std::istream &istream , Plan &plan ) {
    // this is the format we are expecting:
    // 
    // plan
    //      name {name ...}
    //      ...1
    //      semester {semester name}
    //      ...2
    // endplan
    
}