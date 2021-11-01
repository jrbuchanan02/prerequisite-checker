#include "Commands.h++"

#include "Registry.h++"

#include "License.h"

#include <fstream>
#include <iostream>
#include <string>

void printLicense ( int const & , char const *const *const & );
void printVersion ( int const & , char const *const *const & );
void printHelping ( int const & , char const *const *const & );
void startProgram ( int const & , char const *const *const & );

Command commands [ ] = {
    {"--license" , &printLicense},
    {"--version", &printVersion},
    {"--help" , &printHelping},
    {"--filename" , &startProgram},
};

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

void printLicense ( int const & , char const *const *const & ) {
    std::cout << LICENSE << "\n";
}

void printVersion ( int const & , char const *const *const & ) {
    std::cout << "prerequisite-checker version 2\n";
}

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