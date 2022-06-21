/**
 * @file Referred.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Implements a Referred.
 * @version 1
 * @date 2022-06-21
 * 
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 * 
 */

#include <Reference.h++>
#include <Serial.h++>
#include <Referred.h++>

#include <istream>

void Referred::grabReference(std::istream &istream)
{
    istream >> reference;
}

Reference const &Referred::getReference ( ) const noexcept { return reference; }