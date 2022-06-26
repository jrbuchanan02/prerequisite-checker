/**
 * @file Keywords.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Implements the functions found in keywords.
 * @version 1
 * @date 2022-06-25
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */

#include <string>
#include <vector>

#include <Keywords.h++>

std::vector< std::string > types = {
        keywords::types::course,
        keywords::types::requisites,
        keywords::types::semester,
        keywords::types::plan,
        keywords::types::manifest,
};

std::vector< std::string > begins = { };
std::vector< std::string > ends   = { };

std::vector< std::string > const &keywords::types::types ( ) noexcept
{
    return ::types;
}

std::vector< std::string > const &keywords::types::begins ( ) noexcept
{
    if ( ::begins.size ( ) != ::types.size ( ) )
    {
        ::begins.clear();
        std::for_each ( ::types.begin ( ),
                        ::types.end ( ),
                        [ & ] ( std::string s ) {
                            ::begins.push_back ( beginOf ( s ) );
                        } );
        ::begins.shrink_to_fit();
    }
    return ::begins;
}

std::vector< std::string > const &keywords::types::ends ( ) noexcept
{
    if ( ::ends.size ( ) != ::types.size ( ) )
    {
        ::ends.clear();
        std::for_each ( ::types.begin ( ),
                        ::types.end ( ),
                        [ & ] ( std::string s ) {
                            ::ends.push_back ( endOf ( s ) );
                        } );
        ::ends.shrink_to_fit();
    }
    return ::ends;
}

std::string keywords::types::beginOf ( std::string const &string )
{
    return prefixes::begin + string;
}

std::string keywords::types::endOf ( std::string const &string )
{
    return prefixes::end + string;
}

std::vector< std::string >::const_iterator keywords::types::typesBegin ( ) noexcept
{
    return keywords::types::types().begin ( );
}
std::vector< std::string >::const_iterator keywords::types::typesEnd ( ) noexcept
{
    return keywords::types::types().end ( );
}
std::vector< std::string >::const_iterator keywords::types::beginsBegin ( ) noexcept
{
    return ::keywords::types::begins().begin ( );
}
std::vector< std::string >::const_iterator keywords::types::beginsEnd ( ) noexcept
{
    return ::keywords::types::begins ( ).end ( );
}
std::vector< std::string >::const_iterator keywords::types::endsBegin ( ) noexcept
{
    return ::keywords::types::ends ( ).begin ( );
}
std::vector< std::string >::const_iterator keywords::types::endsEnd ( ) noexcept
{
    return ::keywords::types::ends ( ).end ( );
}