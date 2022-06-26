/**
 * \file Commnds.h++
 * \brief Contains the interface main uses to start parsing arguments.
 * \author Joshua Buchanan
 * \date 2021-11-01 (file creation) 2021-11-02 (last modified / documented)
 * \version 1
 * \copyright Joshua Buchanan (C) 2021; MIT License
 */

#pragma once

#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

class Application
{
    bool verbose = false;

    std::ostream mutable *cout = &std::cout;
    std::istream mutable *cin  = &std::cin;

    std::stringstream mutable *dummy     = nullptr;
    std::string                filename  = "./prerequisite-checker";
    std::string reconstructedCommandline = "./prerequisite-checker";

    int                argc = -1;
    char const *const *argv = nullptr;

    std::ifstream openManifest ( std::string const & );
public:
    void handleVerbose ( int const & );
    void handleHelping ( int const & );
    void handleRunning ( int const & );
    void handleListing ( int const & );
    void handleLicense ( int const & );
    void handleVersion ( int const & );

    Application ( ) noexcept = default;

    Application ( int const, char const *const *const );

    std::ostream &getCout ( ) const noexcept;
    std::istream &getCin ( ) const noexcept;

    std::ostream &getLog ( ) const noexcept;

    void run ( );
};
