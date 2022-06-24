/**
 * @file Reference.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Implements a Reference.
 * @version 1
 * @date 2022-06-21
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 *
 */

#include <main.h++>
#include <Reference.h++>
#include <Serial.h++>

#include <istream>
#include <string>

void Reference::extract(ExtractedItem const &item)
{
    // find the first element called "ref"
    try
    {
        auto firstRef = std::find_if(item.begin(), item.end(), [](auto t)
                                 { return t.key == "ref"; });
    contents = firstRef->val;
    } catch ( std::bad_alloc &badAlloc)
    {
        application.getCout() << "Caught a bad_alloc exception.\n";
        application.getCout() << "This is an error. Dumping the offending ExtractedItem's tags.\n";
        for ( auto tag : item)
        {
            application.getCout() << tag.key << " => " << tag.val << "\n";
        }
        throw badAlloc;
    }
}

Reference::Reference(std::string const &contents) noexcept : contents{contents} {}

std::string const Reference::getName() const noexcept { return contents; }
