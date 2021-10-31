/**
 * \file main.c++
 * \brief the entry point for the progam, as MSVS would say, program execution begins and ends here
 * \author Joshua Buchanan
 * \copyright Joshua Buchanan (C) 2021; MIT License
 * \date 2021-10-31 (Happy Halloween!)
 */

#include <iostream>
#include <string>

#include "License.h++"

int main ( int const argc , char const *const *const argv ) {
    if (argc == 2 && std::string ( argv[ 1 ] )  == std::string ( "--license" ) ) {
        std::cout << __LICENSE_TEXT__ << std::endl;
        std::exit ( 0 );
    }
}