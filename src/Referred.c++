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
#include <Referred.h++>
#include <Serial.h++>

#include <istream>

Reference &Referred::getReference() noexcept { return reference; }

Reference const &Referred::getReference() const noexcept { return reference; }