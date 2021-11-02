/**
 * \file Commands.c++
 * \brief Contains the commands, argument parsing logic, and main program code
 * \author Joshua Buchanan
 * \date 2021-11-01 (file creation) 2021-11-02 (last modified / documented)
 * \version 1
 * \copyright Joshua Buchanan (C) 2021; MIT License
 */
#include "Commands.h++"

// we need access to the registry
#include "Registry.h++"
// we need access to the license
#include "License.h"

#include <fstream>
#include <iostream>
#include <string>

typedef void ( *CommandFunction ) ( int const & , char const *const *const & );

/**
 * \struct Command
 * \brief A Command
 * \note Command interprets commandline arguments and switches
 */
struct Command {
    /**
     * \brief the keyword this command responds to
     */
    char const *const keyword;
    /**
     * \brief the function to call if we find this keyword
     */
    CommandFunction   command;
};


void printLicense ( int const & , char const *const *const & );
void printVersion ( int const & , char const *const *const & );
void printHelping ( int const & , char const *const *const & );
void startProgram ( int const & , char const *const *const & );

/**
 * \brief the recognized command-line arguments
 */
Command commands [ ] = {
    {"--license" , &printLicense},
    {"--version", &printVersion},
    {"--help" , &printHelping},
    {"--filename" , &startProgram},
};

/**
 * \function parseArgs
 * \brief Parses argc and argv
 * \param argc the amount of arguments in argv
 * \param argv an array of strings that represent our filename and our commandline
 */
void parseArgs ( int const argc , char const *const *const argv ) {
    std::cout << argv [ 0 ] << std::endl;
    if ( argc < 2 ) {
        printHelping ( argc , argv );
    } else {
        for ( int i = 1; i < argc; i++ ) {
            for ( auto j = 0LLU; j < sizeof ( commands ) / sizeof ( Command ); j++ ) {
                if ( std::string ( commands [ j ].keyword ) == argv [ i ] ) {
                    commands [ j ].command ( argc , argv );
                }
            }
        }
    }
}

/**
 * \function printLicense
 * \brief outputs the MIT License to stdout
 */
void printLicense ( int const & , char const *const *const & ) {
    std::cout << LICENSE << "\n";
}

/**
 * \function printVersion
 * \brief outputs the executable name and the current version to stdout
 */
void printVersion ( int const & , char const *const *const & ) {
    std::cout << "prerequisite-checker version 2\n";
}

/**
 * \function printHelping
 * \brief prints information on the program -- more specifically, program usage.
 * \param argc unused, but we need something to "pass" to printVersion
 * \param argv unused, but we need something to "pass" to printVersion
 */
void printHelping ( int const &argc , char const *const *const &argv ) {
    printVersion ( argc , argv );
#ifdef __WINDOWS__
    static char const *const filename = ".\\prerequisite-checker.exe";
#else
    static char const *const filename = "./prerequisite-checker";
#endif
    std::cout << "Usage: " << filename << " [--license] [--version] [--help] [--filename {manifest filename}]\n";
    std::cout << "At least one argument (the things in the square brackets) must be specified. Manifest filename refers to\n";
    std::cout << "a manifest of where to find all the data files for the program.\n";
}

/**
 * \function startProgram
 * \brief loads all information into a registry and then checks all plans to see if they are valid
 * \param argc argc (the one passed to main)
 * \param argv argv (the one passed to main)
 */
void startProgram ( int const &argc , char const *const *const &argv ) {
    // find what "i" was.
    auto i = 0LLU;
    for ( ; std::string ( "--filename" ) != argv [ i ]; i++ );
    if ( i + 1 - argc <= 0 ) {
        std::cout << "The --filename option needs an argument.\n";
        std::exit ( -1 );
    } else {
        char const *const filename = argv [ i + 1 ];
        std::fstream file ( filename );
        Registry registry;
        file >> registry;
        registry.runTests ( );
    }
    //std::cout << "This option is currently under construction...\n";
    //std::cout << "The main functionality of the program will go here.\n";
}