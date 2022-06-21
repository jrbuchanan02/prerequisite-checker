#pragma once

#include "Serial.h++"

#include <istream>
#include <string> 
#include <sstream>

typedef void ( *KeywordInterpreter ) ( std::istream & , Serial & , bool & );

struct Keyword {
    char const *const word;
    KeywordInterpreter action;
};


bool const extractToKeyword ( std::istream & , std::string const & );

static constexpr char const *const keywords [] = {
    "begin", // begins a declaration
    "end", // ends a declaration
    "ref", // marks a reference
    "#", // marks a comment
    "req", // marks a requisite, can be modified by the flags "pre" and "con"
    "reqs", // marks a reference to a requisite group
    "hours", // indicates the number of hours in a course
    "name", // indicates that the rest of the line contains the name of the course. If multiple name keywords are present,
            // the contents are concatinated in the order seen.
    "desc", // indicates that the rest of the line contains the description of the course. If multiple desc keywords are
            // present, the contents are concatinated in the order seen
    "semester", // a semester in a plan. Followed by a reference to the semester then a list of course references finishing the line
};

void inline interpretComment ( std::istream & , Serial & , bool & ) { return; }
void inline interpretEnd ( std::istream & , Serial & , bool &endFlag ) { 
    endFlag = true;
}

void inline extract ( Keyword usedKeywords [ ] , std::istream &inputstream , Serial &serial ) {
    std::stringstream line;
    std::string firstWord;
    do {
        std::getline ( inputstream , firstWord );
        line = std::stringstream ( firstWord );
        line >> firstWord;
    } while ( firstWord != usedKeywords [ 0 ].word );
    bool end = false;
    usedKeywords [ 0 ].action ( line , serial , end );
    while (!end ) {
        std::getline ( inputstream , firstWord );
        line = std::stringstream ( firstWord );
        line >> firstWord;
        for ( std::size_t i = 0; i < sizeof ( keywords) / sizeof ( keywords [ 0] ); i++) {
            if ( firstWord == usedKeywords [ i ].word ) {
                usedKeywords [ i ].action ( line , serial , end );
                break;
            }
        }
    }
}