#pragma once

#include <iostream>

typedef void ( *CommandFunction ) ( int const & , char const *const *const & );


struct Command {
    char const *const keyword;
    CommandFunction   command;
};

void parseArgs ( int const , char const *const *const );