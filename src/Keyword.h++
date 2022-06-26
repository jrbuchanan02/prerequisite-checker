#pragma once

#include <functional>
#include <sstream>
#include <string>

#include "Referred.h++"
#include "Serial.h++"

struct Keyword {
    std::string keyword;
    std::function < void ( std::stringstream & , Serial & ) > function;
};

std::function < void ( std::stringstream & , Serial & ) > wrap ( std::function < void ( std::stringstream & , Referred & ) > function );

void referenceFunction ( std::stringstream &stringstream , Referred &referred );