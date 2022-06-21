/**
 * @file Reference.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Implements a Reference.
 * @version 1
 * @date 2022-06-21
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 *
 */

#include <Reference.h++>
#include <Serial.h++>

#include <istream>
#include <string>

std::istream &Reference::extract(std::istream &istream)
{
    std::getline(istream, contents);
    while (contents.starts_with(" "))
    {
        contents = contents.substr(1);
    }
    return istream;
}

Reference::Reference(std::string const &contents) noexcept : contents{contents} {}

std::string const Reference::getName() const noexcept { return contents;}

