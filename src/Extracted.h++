/**
 * @file Extracted.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Something containing keys and values extracted from a file.
 * @version 1
 * @date 2022-06-24
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */

#pragma once

#include <Keywords.h++>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

class Extracted;
using ExtractedPointer = std::shared_ptr< Extracted >;

struct Tag
{
    std::string key = "";
    std::string val = "";
};

using ExtractedItem = std::vector< Tag >;

ExtractedItem filterForTagType ( ExtractedItem const &, std::string const & );

class Extracted
{
    std::vector< ExtractedItem > allTags;
protected:
    inline std::vector< ExtractedItem > &getAllTags ( ) noexcept
    {
        return allTags;
    }
public:
    inline std::vector< ExtractedItem > getTagsOfType ( std::string type ) const
    {
        std::vector< ExtractedItem > tags;
        for ( auto extractedItem : allTags )
        {
            bool match = false;
            for ( auto tag : extractedItem )
            {
                using namespace keywords;
                if ( tag.key == keys::type and tag.val == type )
                {
                    match = true;
                    break;
                }
            }
            if ( match )
            {
                tags.push_back ( extractedItem );
            }
        }
        return tags;
    }

    virtual void extractFrom ( std::istream & ) = 0;

    inline Extracted &operator+= ( Extracted const &that )
    {
        for ( auto extractedItem : that.allTags )
        {
            allTags.push_back ( extractedItem );
        }
        return *this;
    }
};

class PCFExtracted : public Extracted
{
    void extractSingle ( std::istream &i );
public:
    virtual void extractFrom ( std::istream &i );
};

class YAMLExtracted : public Extracted
{
    void extractNode ( YAML::Node );

    std::string extractString ( YAML::Node );
    std::string extractString ( YAML::Node, std::string );
    std::string extractType ( YAML::Node );

    std::vector< std::string > extractSequence ( YAML::Node, std::string );
    std::vector< std::string > extractSequence ( YAML::Node );

    void assertIsMap ( YAML::Node, std::string );

    void courseExtraction ( YAML::Node, ExtractedItem & );
    void requisitesExtraction ( YAML::Node, ExtractedItem & );
    void semesterExtraction ( YAML::Node, ExtractedItem & );
    void planExtraction ( YAML::Node, ExtractedItem & );
    void manifestExtraction ( YAML::Node, ExtractedItem & );

    bool extractOptional ( YAML::Node, std::string, bool = false );

    void extractSingleRequisiteEntry ( YAML::Node, ExtractedItem & );

    void notifyIncorrectFormat ( std::string );

    void extractDatum(YAML::Node);
public:
    virtual void extractFrom ( std::istream &i );
};

ExtractedPointer open ( std::string const & );