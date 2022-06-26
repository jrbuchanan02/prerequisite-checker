/**
 * \file Commands.c++
 * \brief Contains the commands, argument parsing logic, and main program code
 * \author Joshua Buchanan
 * \date 2021-11-01 (file creation) 2021-11-02 (last modified / documented)
 * \version 1
 * \copyright Joshua Buchanan (C) 2021; MIT License
 */

#include <Commands.h++>
#include <Extracted.h++>
#include <Keywords.h++>
#include <License.h>
#include <Registry.h++>

#include <fstream>
#include <functional>
#include <iostream>
#include <string>

using CommandFunction = std::function< void ( Application *, int const & ) >;

using keywords::delimiters::commandlineCourseDelimiter;
using keywords::delimiters::statementDelimiter;

struct Command
{
    CommandFunction command;
    std::string     keyword;
    std::string     description;
    bool            priority = false;
};

std::string getLicenseDescription ( );
std::string getVersionDescription ( );
std::string getHelpingDescription ( );
std::string getRunningDescription ( );
std::string getListingDescription ( );
std::string getVerboseDescription ( );

/**
 * \brief the recognized command-line arguments
 */
Command commands [] = {
        { std::mem_fn ( &Application::handleLicense ),
          "--license",
          getLicenseDescription ( ),
          true },
        { std::mem_fn ( &Application::handleVersion ),
          "--version",
          getVersionDescription ( ),
          true },
        { std::mem_fn ( &Application::handleHelping ),
          "--help",
          getHelpingDescription ( ) },
        { std::mem_fn ( &Application::handleRunning ),
          "--filename",
          getRunningDescription ( ) },
        { std::mem_fn ( &Application::handleListing ),
          "--detail",
          getListingDescription ( ) },
        { std::mem_fn ( &Application::handleVerbose ),
          "--verbose",
          getVerboseDescription ( ),
          true },
};

Application::Application ( int const argc, char const *const *const argv )
{
    this->argc = argc;
    this->argv = argv;

    filename   = argv [ 0 ];
    char slash = '/';
    if ( filename.find ( '\\' ) != std::string::npos )
    {
        slash = '\\';
    }
    std::unsigned_integral auto position = filename.find_last_of ( slash );
    filename                             = "." + filename.substr ( position );

    reconstructedCommandline = "";
    std::for_each ( &argv [ 0 ], &argv [ argc ], [ & ] ( auto cmd ) {
        reconstructedCommandline += std::string { cmd } + " ";
    } );

    dummy = new std::stringstream { };
}

void Application::run ( )
{
    if ( argc < 2 )
    {
        handleHelping ( -1 );
    } else
    {
        // run this loop twice. First go through everything that is
        // "high priority" then go through everything that is not
        // high priority.
        for ( std::integral auto checks = 0; checks < 2; checks++ )
        {
            bool highPriority = false;
            if ( checks == 0 )
            {
                highPriority = true;
            }
            for ( std::integral auto i = 1; i < argc; i++ )
            {
                std::for_each ( std::begin ( commands ),
                                std::end ( commands ),
                                [ & ] ( auto command ) {
                                    if ( highPriority xor command.priority )
                                    {
                                        return;
                                    }
                                    if ( command.keyword == argv [ i ] )
                                    {
                                        command.command ( this, i );
                                    }
                                } );
            }
        }
    }
}

std::ifstream Application::openManifest ( std::string const &s )
{
    return std::ifstream { s };
}

void Application::handleLicense ( int const & )
{
    getCout ( ) << LICENSE << statementDelimiter;
}

void Application::handleVersion ( int const & )
{
    getCout ( ) << "prerequisite-checker version 3" << statementDelimiter;
}

void Application::handleHelping ( int const &c )
{
    handleVersion ( c );

    // two modes. If argc has a command name following it, print information
    // on that command in particular.

    if ( c < 0 or c + 1 == argc )
    {
        getCout ( ) << "Usage: " << filename << " [commands]"
                    << statementDelimiter;
        getCout ( ) << "Where [commands] is at least one of: "
                    << statementDelimiter;
        std::for_each ( std::begin ( commands ),
                        std::end ( commands ),
                        [ & ] ( auto command ) {
                            getCout ( ) << "- " << command.keyword
                                        << statementDelimiter;
                        } );
    } else
    {
        std::string requested          = argv [ c + 1 ];
        auto        describingFunction = [ & ] ( Command command ) {
            if ( requested == command.keyword )
            {
                getCout ( ) << "Usage: " << filename << command.keyword
                            << " {arguments}" << statementDelimiter;
                getCout ( ) << "Description: " << command.description;
                if ( command.description == "" )
                {
                    getCout ( ) << "[None Provided.]";
                }
                getCout ( ) << statementDelimiter;
            }
        };

        std::for_each ( std::begin ( commands ),
                        std::end ( commands ),
                        describingFunction );
    }
}

void Application::handleRunning ( int const &c )
{
    std::integral auto filePosition = c + 1;
    if ( filePosition >= argc )
    {
        getCout ( ) << "The --filename option needs an argument."
                    << statementDelimiter;
        std::exit ( -1 );
    }
    std::ifstream    specifiedFile = openManifest ( argv [ filePosition ] );
    ExtractedPointer extraction    = open ( argv [ filePosition ] );
    getLog ( ) << "Beginning extraction..." << statementDelimiter;
    extraction->extractFrom ( specifiedFile );
    getLog ( ) << "Extraction complete, parsing registry..."
               << statementDelimiter;
    Registry registry;
    registry.parse ( *extraction );
    getLog ( ) << "Running tests." << statementDelimiter;
    registry.runTests ( );
}

void Application::handleListing ( int const &c )
{
    std::integral auto filePosition     = c + 1;
    std::integral auto firstArgPosition = c + 2;
    if ( filePosition >= argc )
    {
        getCout ( ) << "The --detail option needs at least two arguments. "
                       "Prerequisite Checker received no arguments."
                    << statementDelimiter;
        std::exit ( -1 );
    }
    if ( firstArgPosition >= argc )
    {
        getCout ( ) << "The --detail option needs at least two arguments. "
                       "Prerequisite Checker received only one argument."
                    << statementDelimiter;
        std::exit ( -1 );
    }
    std::ifstream    specifiedFile = openManifest ( argv [ filePosition ] );
    ExtractedPointer extraction    = open ( argv [ filePosition ] );
    getLog ( ) << "Beginning extraction..." << statementDelimiter;
    extraction->extractFrom ( specifiedFile );
    getLog ( ) << "Extraction complete, parsing registry..."
               << statementDelimiter;
    Registry registry;
    registry.parse ( *extraction );

    auto getArgument = [ & ] ( int const &index ) -> std::string {
        return std::string { argv [ c + index ] };
    };

    bool listAll = getArgument ( 2 ) == "all";

    for ( auto courseReference : registry.knownCourses ( ) )
    {
        getLog ( ) << "Checking if details requested for course "
                   << courseReference.getName ( ) << statementDelimiter;
        bool requested =
                reconstructedCommandline.find ( courseReference.getName ( ) )
                != std::string::npos;
        if ( listAll or requested )
        {
            getLog ( ) << "Details were requested." << statementDelimiter;
            CoursePointer course = registry.resolveCourse ( courseReference );

            getCout ( ) << "Details on course " << courseReference << ": ";
            if ( not course )
            {
                getCout ( ) << "[No Details]" << statementDelimiter;
                continue;
            } else
            {
                getCout ( ) << statementDelimiter;
            }
            getCout ( ) << "Name: " << course->getName ( )
                        << statementDelimiter;
            getCout ( ) << "Hours: " << course->getHours ( )
                        << statementDelimiter;
            getCout ( ) << "Description: " << course->getDesc ( )
                        << statementDelimiter;
            getCout ( ) << "Tags: ";
            for ( auto flag : *course ) { getCout ( ) << flag << "; "; }
            getCout ( ) << statementDelimiter;

            getCout ( ) << "Requisites: ";
            auto requisites = course->resolveRequisites ( registry );
            if ( requisites.empty ( ) )
            {
                getCout ( ) << "None." << statementDelimiter;
            } else
            {
                getCout ( ) << " (All lines must be satisfied)"
                            << statementDelimiter;
                for ( auto requisitesPointer : requisites )
                {
                    std::string reference =
                            ( ( Requisites const ) *requisitesPointer )
                                    .getReference ( )
                                    .getName ( );
                    getCout ( ) << reference << ": ";
                    auto requisiteList = requisitesPointer->getRequisites ( );
                    for ( auto requisite = requisiteList.begin ( );
                          requisite != requisiteList.end ( );
                          requisite++ )
                    {
                        getCout ( ) << requisite->getCourse ( );
                        std::string appended = " ";
                        if ( requisite->allowConcurrent ( )
                             or requisite->allowPreviously ( ) )
                        {
                            appended += "(";
                            if ( requisite->allowPreviously ( ) )
                            {
                                appended += "P";
                            }
                            if ( requisite->allowConcurrent ( ) )
                            {
                                appended += "C";
                            }
                            appended += ") ";
                        }
                        getCout ( ) << appended;
                        auto temp = requisite;
                        if ( temp++ != requisiteList.end ( ) )
                        {
                            getCout ( ) << " "
                                        << keywords::delimiters::
                                                   requisiteCourseDelimiter
                                        << " ";
                        } else
                        {
                            getCout ( ) << statementDelimiter;
                        }
                    }
                }
            }
            break;
        } else
        {
            getLog ( ) << "Details were not requested." << statementDelimiter;
        }
    }
}

void Application::handleVerbose ( int const &argc ) { verbose = true; }

std::ostream &Application::getCout ( ) const noexcept { return *cout; }

std::ostream &Application::getLog ( ) const noexcept
{
    return verbose ? *cout : *dummy;
}

std::istream &Application::getCin ( ) const noexcept { return *cin; }

std::string getLicenseDescription ( )
{
    return "Prints Prerequisite Checker's license." + statementDelimiter;
}

std::string getVersionDescription ( )
{
    return "Prints Prerequisite Checker's verison." + statementDelimiter;
}

std::string getHelpingDescription ( )
{
    return "Prints the usage of Prerequisite Checker and its commands."
         + statementDelimiter;
}

std::string getRunningDescription ( )
{
    return "Runs the program and tests all plans found." + statementDelimiter;
}

std::string getListingDescription ( )
{
    return "Lists details about one or multiple courses. Passing with \"all\" "
           "lists all known courses."
         + statementDelimiter;
    ;
}

std::string getVerboseDescription ( )
{
    return "Turns on logging for debugging purposes. For the program to "
           "actually then do something, pass with another argument."
         + statementDelimiter;
}