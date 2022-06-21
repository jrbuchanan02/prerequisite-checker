#pragma once

#include <Serial.h++>

#include <functional>
#include <istream>
#include <sstream>
#include <string>

using KeywordInterpreter = std::function<void(std::istream &, Serial &, bool &)>;

struct Keyword
{
    std::string word;
    KeywordInterpreter action;
};

bool const extractToKeyword(std::istream &, std::string const &);

std::vector<std::string> getKeywords();

void interpretComment(std::istream &, Serial &, bool &);
void interpretEnd(std::istream &, Serial &, bool &endFlag);

void extract(std::vector<Keyword>, std::istream &, Serial &);