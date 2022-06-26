/**
 * @file Reference.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Implements a Reference.
 * @version 1
 * @date 2022-06-21
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */

#include <Keywords.h++>
#include <Reference.h++>
#include <Serial.h++>
#include <main.h++>

#include <istream>
#include <string>

using keywords::keys::ref;

void Reference::extract ( ExtractedItem const &item )
{
    auto refTags = filterForTagType ( item, ref );
    contents     = refTags.front ( ).val;
    // while ( contents.starts_with ( " " ) ) { contents = contents.substr ( 1 ); }
    // while ( contents.ends_with ( " " ) )
    // {
    //     contents = contents.substr ( 0, contents.size ( ) );
    // }
}

Reference::Reference ( std::string const &contents ) noexcept :
        contents { contents }
{ }

std::string const Reference::getName ( ) const noexcept { return contents; }
