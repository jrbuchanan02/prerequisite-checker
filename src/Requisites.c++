#include <Course.h++>
#include <Keywords.h++>
#include <Plan.h++>
#include <Referred.h++>
#include <Registry.h++>
#include <Requisite.h++>
#include <Requisites.h++>

#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

using keywords::delimiters::statementDelimiter;
using keywords::keys::req;
using keywords::keys::reqs;

void Requisites::extract ( ExtractedItem const &extracted )
{
    application.getLog ( ) << "Extracting Requisites at 0x" << ( void * ) this
                           << statementDelimiter;
    throwOnWrongType ( reqs, extracted );
    Referred::extract ( extracted );
    // extract everything...
    std::intmax_t counter = 0;

    auto generateKeyStart = [ & ] ( ) -> std::string {
        return req + " " + std::to_string ( counter );
    };

    auto hasAny = [ & ] ( ) {
        std::string keyStart = generateKeyStart ( );
        auto        first    = std::find_if (
                extracted.begin ( ),
                extracted.end ( ),
                [ & ] ( auto t ) { return t.key.starts_with ( keyStart ); } );
        return first != extracted.end ( );
    };
    for ( ; hasAny ( ); counter++ )
    {
        application.getLog ( ) << "\tChecking for group "
                               << generateKeyStart ( ) << statementDelimiter;
        // find all requisites matching this group.
        ExtractedItem filtered;
        std::string   keyStart = generateKeyStart ( );
        for ( Tag tag : extracted )
        {
            if ( tag.key.starts_with ( keyStart ) )
            {
                application.getLog ( ) << "\tFound group named " << tag.key
                                       << statementDelimiter;
                filtered.push_back ( tag );
            }
        }
        for ( std::intmax_t i = 0; Tag tag : filtered )
        {
            application.getLog ( ) << "\tParsing group named " << tag.key
                                   << statementDelimiter;
            Requisite found;
            found.counter = i++;
            found.extract ( filtered );
            requisites.push_back ( found );
        }
    }
}

bool const Requisites::meetsRequisite ( Course const &course ) const noexcept
{
    for ( Requisite requisite : requisites )
    {
        application.getLog ( )
                << "Checking if " << requisite.getCourse ( ) << " is "
                << course.getReference ( ) << statementDelimiter;
        if ( requisite.getCourse ( ) == course.getReference ( ) )
        {
            application.getLog ( )
                    << "Found that " << requisite.getCourse ( ) << " is "
                    << course.getReference ( ) << statementDelimiter;
            return true;
        }
    }
    application.getLog ( ) << "Could not match " << course.getReference ( )
                           << statementDelimiter;
    return false;
}

std::vector< Requisite > const &Requisites::getRequisites ( ) const noexcept
{
    return requisites;
}