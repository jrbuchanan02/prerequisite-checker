/**
 * @file Extracted.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Implements an Extracted.
 * @note YAML-cpp lies within the repository just for this file.
 * @version 1
 * @date 2022-06-24
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <Extracted.h++>
#include <Keywords.h++>
#include <main.h++>

#include <yaml-cpp/yaml.h>

using namespace keywords::delimiters;
using namespace keywords::keys;
using namespace keywords::types;
using namespace keywords::prefixes;
using namespace keywords::tags;
using namespace keywords::manipulators;

std::string pcfExtensions [] = {
        "txt",
        "pcf",
        "dat",
};

std::string ymlExtensions [] = {
        "yaml",
        "yml",
        "json",
};

bool matchesFileExtension ( std::string const               &filename,
                            std::random_access_iterator auto begin,
                            std::random_access_iterator auto end )
{
    bool found = false;
    std::for_each ( begin, end, [ & ] ( auto item ) {
        if ( filename.ends_with ( item ) )
        {
            found = true;
        }
    } );
    return found;
}

bool isPCFFile ( std::string const &filename )
{
    return matchesFileExtension ( filename,
                                  std::begin ( pcfExtensions ),
                                  std::end ( pcfExtensions ) );
}

bool isYMLFile ( std::string const &filename )
{
    return matchesFileExtension ( filename,
                                  std::begin ( ymlExtensions ),
                                  std::end ( ymlExtensions ) );
}

ExtractedPointer open ( std::string const &filename )
{
    if ( isPCFFile ( filename ) )
    {
        return ExtractedPointer ( new PCFExtracted ( ) );
    }
    if ( isYMLFile ( filename ) )
    {
        return ExtractedPointer ( new YAMLExtracted ( ) );
    }
    return nullptr;
}

// extracts a single element from a single file.
void PCFExtracted::extractSingle ( std::istream &i )
{
    application.getLog ( ) << "\tAttempting to extract possible tag."
                           << statementDelimiter;
    std::string line = "";

    auto getLine = [ & ] ( ) {
        std::getline ( i, line, statementDelimiter.front ( ) );
    };

    bool        foundSomething = false;
    std::string foundKey       = "";
    while ( not foundSomething and not i.eof ( ) )
    {
        application.getLog ( )
                << "\tGrabbing next available line..." << statementDelimiter;
        getLine ( );
        application.getLog ( )
                << "\tRead \"" << line << "\". Looking for something..."
                << statementDelimiter;
        for ( auto begin : begins ( ) )
        {
            if ( line.starts_with ( begin ) )
            {
                foundKey       = begin;
                foundSomething = true;
                break;
            }
        }
    }
    if ( not foundSomething )
    {
        application.getLog ( ) << "\tDid not find tag." << statementDelimiter;
        return;
    }
    // FIXME #1: PCFExtracted sometimes finds data tags that do not exist. There
    // is a workaround below where we do not add the extracted item if there is
    // only one element, but, we still go through the logic of parsing the
    // key to find the ghost tag.
    while ( not i.eof ( ) )
    {
        application.getLog ( ) << "\tLine marks beginning of a " << foundKey
                               << statementDelimiter;
        getAllTags ( ).push_back ( { } );
        ExtractedItem &item = getAllTags ( ).back ( );
        item.push_back ( Tag { type, foundKey } );
        // if we found Requisites, there may be some extra processing...
        bool               foundRequisites = foundKey == beginOf ( requisites );
        bool               foundPlan       = foundKey == beginOf ( plan );
        bool               foundEnd        = false;
        std::integral auto currentRequisiteID = 0;
        while ( not foundEnd and not i.eof ( ) )
        {
            getLine ( );
            std::stringstream lineParser { line };
            std::string       keyword = "";
            lineParser >> keyword;
            application.getLog ( ) << "\tThis line's keyword is \"" << keyword
                                   << "\"" << statementDelimiter;
            if ( keyword.starts_with ( comment ) )
            {
                continue;
            }
            std::string rest = "";
            rest = line.substr ( line.find ( keyword ) + keyword.size ( ) );
            rest = removeAllLeading ( space, rest );
            std::string notAllowedToTrail [] = { " ", "\n" };
            rest = removeAllTrailing ( std::begin ( notAllowedToTrail ),
                                       std::end ( notAllowedToTrail ),
                                       rest );
            if ( keyword == endOf ( foundKey ) )
            {
                foundEnd = true;
            }
            if ( foundRequisites and keyword == req )
            {
                // extra processing here.
                // we need different keys for when a requisite must
                // be taken previously, concurrently, either and if
                // multiple requisites satisfy the same category. For
                // example, Calculus I and Honors Calculus I may appear
                // on the same line and both satisfy the requisite for
                // Calculus II. So, we keep an integral ID of each requisite.
                // if the counter is the same, the courses both satisfy the same
                // set of requirements.
                std::stringstream restParser { rest };
                std::string       id  = std::to_string ( currentRequisiteID++ );
                std::string       key = keyword + space + id;
                application.getLog ( )
                        << "\tKey is now " << key << statementDelimiter;
                while ( restParser )
                {
                    std::string requisite = "";
                    std::string usedKey   = key;
                    while ( restParser
                            and not requisite.ends_with (
                                    requisiteCourseDelimiter ) )
                    {
                        std::string temp = "";
                        restParser >> temp;
                        requisite += " " + temp;
                    }
                    requisite = removeAllLeading ( space, requisite );

                    std::stringstream requisiteParser { requisite };
                    std::string       nextTwo [ 2 ] { "", "" };
                    requisiteParser >> nextTwo [ 0 ] >> nextTwo [ 1 ];
                    if ( eitherEqual ( nextTwo [ 0 ],
                                       nextTwo [ 1 ],
                                       takenPreviously ) )
                    {
                        usedKey += space + takenPreviously;
                    }
                    if ( eitherEqual ( nextTwo [ 0 ],
                                       nextTwo [ 1 ],
                                       takenConcurrent ) )
                    {
                        usedKey += space + takenConcurrent;
                    }
                    // remove leading spaces, leading tags, trailing spaces
                    // and trailing syntax from requisite.
                    std::string requisiteNonstarter [] = { " ",
                                                           takenPreviously,
                                                           takenConcurrent };
                    std::string requisiteNonender []   = {
                              " ",
                              "\n",
                              requisiteCourseDelimiter };

                    requisite = removeAllLeading (
                            std::begin ( requisiteNonstarter ),
                            std::end ( requisiteNonstarter ),
                            requisite );
                    requisite = removeAllTrailing (
                            std::begin ( requisiteNonender ),
                            std::end ( requisiteNonender ),
                            requisite );

                    application.getLog ( )
                            << "\tAdding key/value pair " << usedKey << " and "
                            << requisite << statementDelimiter;
                    item.push_back ( Tag { usedKey, requisite } );
                }

                continue;
            }
            if ( foundPlan and keyword == semester )
            {
                // take rest from beginning to the ending
                // then split each other course reference on the
                // line and add it as a tag.
                std::string key =
                        rest.substr ( 0, rest.find ( planSemesterDelimiter ) );
                application.getLog ( )
                        << "\tKey is now " << key << statementDelimiter;
                std::stringstream restParser {
                        rest.substr ( rest.find ( planSemesterDelimiter )
                                      + planSemesterDelimiter.size ( ) ) };
                application.getLog ( )
                        << "\tParsing all information from "
                        << restParser.str ( ) << statementDelimiter;
                while ( restParser )
                {
                    std::string val = "";
                    while ( restParser
                            and not val.ends_with ( planCourseDelimiter )
                            and not val.ends_with ( requisiteCourseDelimiter ) )
                    {
                        std::string temp = "";
                        restParser >> temp;
                        val += " " + temp;
                        application.getLog ( ) << "\tValue is now " << val
                                               << statementDelimiter;
                    }
                    val = removeTrailing ( requisiteCourseDelimiter, val );
                    val = removeTrailing ( planCourseDelimiter, val );
                    val = removeAllLeading ( space, val );
                    val = removeAllTrailing ( space, val );

                    item.push_back ( Tag { key, val } );
                }
            }
            application.getLog ( ) << "\tAdded key/value pair " << keyword
                                   << " and " << rest << statementDelimiter;
            if ( not foundEnd and not keyword.empty ( ) and not rest.empty ( ) )
            {
                item.push_back ( { keyword, rest } );
            }
        }
        // Sanity check: all ExtractedItems should have at least two tags. One
        // for type and one for reference.
        if ( getAllTags ( ).back ( ).size ( ) < 2 )
        {
            application.getLog ( )
                    << "\tFound empty " << getAllTags ( ).back ( ).front ( ).val
                    << statementDelimiter;
            getAllTags ( ).pop_back ( );
        }
        if ( getAllTags ( ).back ( ).front ( ).key == type
             and getAllTags ( ).back ( ).front ( ).val == manifest )
        {
            application.getLog ( ) << "\tThis data is a manifest, opening all "
                                      "relavent files..."
                                   << statementDelimiter;
            for ( auto tag : getAllTags ( ).back ( ) )
            {
                if ( tag.key == type )
                {
                    continue;
                }
                application.getLog ( ) << "\tReading file " << tag.val << "...";
                auto pointer = open ( tag.val );
                if ( not pointer )
                {
                    application.getLog ( )
                            << " File of unknown format." << statementDelimiter;
                    continue;
                } else
                {
                    application.getLog ( ) << statementDelimiter;
                }
                std::ifstream file { tag.val };
                pointer->extractFrom ( file );
                *this += *pointer;
            }
        }
    }
}

void PCFExtracted::extractFrom ( std::istream &i )
{
    application.getLog ( ) << "Extracting file(s)..." << statementDelimiter;
    while ( not i.eof ( ) )
    {
        application.getLog ( ) << "\tFound Tag?" << statementDelimiter;
        extractSingle ( i );
    }
    application.getLog ( ) << "Extraction finished." << statementDelimiter;
}

ExtractedItem filterForTagType ( ExtractedItem const &items,
                                 std::string const   &key )
{
    ExtractedItem result;
    for ( Tag tag : items )
    {
        if ( tag.key == key )
        {
            result.push_back ( tag );
        }
    }
    return result;
}