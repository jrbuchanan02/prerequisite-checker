/**
 * @file YAMLExtracted.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Specifically the YAMLExtracted class. This class interfaces with
 * yaml-cpp, so it needs more code. Like, lots more code.
 * @version 1
 * @date 2022-06-26
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed
 * above.
 *
 */
#include <Extracted.h++>
#include <Keywords.h++>
#include <main.h++>

#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>

using namespace keywords;

void YAMLExtracted::extractFrom ( std::istream &istream )
{
    auto nodes = YAML::LoadAll ( istream );
    for ( auto node : nodes )
    {
        application.getLog ( )
                << "Extracting document..." << delimiters::statementDelimiter;
        extractNode ( node );
    }
}

void YAMLExtracted::extractNode ( YAML::Node sequence )
{
    application.getLog ( ) << "Extracting sequence..."
                           << delimiters::statementDelimiter;
    if ( not sequence.IsSequence ( ) )
    {
        notifyIncorrectFormat (
                "Prerequisite checker needs a sequence at the root of each "
                "document. The following is not a sequence: "
                + sequence.Tag ( ) );
        return;
    }
    for ( std::unsigned_integral auto i = 0U; i < sequence.size ( ); i++ )
    {
        application.getLog ( )
                << "\tDatum " << i + 1 << " of " << sequence.size ( )
                << delimiters::statementDelimiter;
        extractDatum ( sequence [ i ] );
    }
}

std::string YAMLExtracted::extractType ( YAML::Node node )
{
    if ( not node.IsMap ( ) )
    {
        notifyIncorrectFormat (
                "Prerequisite checker needs a map within each sequence for "
                "each document. The following is not a map: "
                + node.Tag ( ) );
    }
    if ( not node [ keys::type ] )
    {
        notifyIncorrectFormat (
                "Prerequisite checker needs to know the type of each data "
                "block (the maps). The following is missing a member tagged \""
                + keys::type + "\": " + node.Tag ( ) );
    }
    std::string result = "";
    try
    {
        result = node [ keys::type ].as< std::string > ( );
    } catch ( YAML::Exception &formatException )
    {
        notifyIncorrectFormat (
                "Prerequisite Checker could not (by default, at least) read "
                "the string from \""
                + node [ keys::type ].Scalar ( ) + "\"." );
    }
    return result;
}

void YAMLExtracted::courseExtraction ( YAML::Node node, ExtractedItem &item )
{
    // our string values.
    std::string                reference = extractString ( node, keys::ref );
    std::string                name      = extractString ( node, keys::name );
    std::string                desc      = extractString ( node, keys::desc );
    std::string                hour      = extractString ( node, keys::hours );
    // our sequence values.
    std::vector< std::string > reqs      = extractSequence ( node, keys::reqs );
    std::vector< std::string > tags = extractSequence ( node, keys::tagged );

    item.push_back ( Tag { keys::ref, reference } );
    item.push_back ( Tag { keys::name, name } );
    item.push_back ( Tag { keys::desc, desc } );
    item.push_back ( Tag { keys::hours, hour } );
    for ( auto req : reqs ) { item.push_back ( Tag { keys::reqs, req } ); }
    for ( auto tag : tags ) { item.push_back ( Tag { keys::tagged, tag } ); }
}

void YAMLExtracted::requisitesExtraction ( YAML::Node     node,
                                           ExtractedItem &item )
{
    std::string reference = extractString ( node, keys::ref );
    if ( not node [ keys::req ] )
    {
        notifyIncorrectFormat (
                "This requisites must have requisites within it." );
        return;
    }
    if ( not node [ keys::req ].IsSequence ( ) )
    {
        notifyIncorrectFormat (
                "All requisites in a requisite group must be in a sequence." );
        return;
    }
    for ( std::unsigned_integral auto i = 0U; i < node [ keys::req ].size ( );
          i++ )
    {
        extractSingleRequisiteEntry ( node [ keys::req ][ i ], item );
    }
}

void YAMLExtracted::semesterExtraction ( YAML::Node node, ExtractedItem &item )
{
    std::string                reference = extractString ( node, keys::ref );
    std::vector< std::string > tags = extractSequence ( node, keys::tagged );
    item.push_back ( Tag { keys::ref, reference } );
    for ( auto tag : tags ) { item.push_back ( Tag { keys::tagged, tag } ); }
}

void YAMLExtracted::planExtraction ( YAML::Node node, ExtractedItem &item )
{
    std::string reference = extractString ( node, keys::ref );
    for ( auto iterator = node.begin ( ); iterator != node.end ( ); iterator++ )
    {
        std::string semesterName = extractString ( iterator->first );
        if ( semesterName == keys::ref )
        {
            continue;
        }
        std::vector< std::string > courses =
                extractSequence ( node, semesterName );
        for ( auto course : courses )
        {
            item.push_back ( Tag { semesterName, course } );
        }
    }
}

void YAMLExtracted::manifestExtraction ( YAML::Node node, ExtractedItem &item )
{
    std::vector< std::string > files = extractSequence ( node );
    for ( auto file : files )
    {
        *this += *open ( file );
        item.push_back ( Tag { yaml::manifestEntry, file } );
    }
}

void YAMLExtracted::extractSingleRequisiteEntry ( YAML::Node     node,
                                                  ExtractedItem &item )
{
    std::string   key = keys::req + delimiters::space;
    std::intmax_t id  = 0;
    try
    {
        id = node [ yaml::requisiteID ].as< std::intmax_t > ( );
    } catch ( YAML::Exception & )
    {
        std::string specifics = "";
        if ( node [ yaml::requisiteID ] )
        {
            specifics = "with scalar \"" + node [ yaml::requisiteID ].Scalar ( )
                      + "\".";
        } else
        {
            specifics = "that cannot be read.";
        }
        notifyIncorrectFormat (
                "All requisite entries in a requisite group must have an "
                "integral ID. This one has an ID "
                + specifics );
        return;
    }

    bool previously = extractOptional ( node, tags::takenPreviously, false );
    bool concurrent = extractOptional ( node, tags::takenConcurrent, false );

    key += std::to_string ( id );
    if ( previously )
    {
        key += delimiters::space + tags::takenPreviously;
    }
    if ( concurrent )
    {
        key += delimiters::space + tags::takenConcurrent;
    }
    key             = manipulators::removeAllTrailing ( " ", key );
    std::string val = extractString ( node, keys::ref );
    if ( val.empty ( ) )
    {
        notifyIncorrectFormat (
                "Detected that the course reference for this requisite "
                "could not be read. Not adding this requisite to the "
                "group." );
        return;
    }
    item.push_back ( Tag { key, val } );
}

std::string YAMLExtracted::extractString ( YAML::Node node )
{
    std::string result = "";
    if ( node.IsSequence ( ) )
    {
        for ( std::unsigned_integral auto i = 0U; i < node.size ( ); i++ )
        {
            result += node [ i ].as< std::string > ( ) + " ";
        }
        result = manipulators::removeAllTrailing ( " ", result );
    } else
    {
        result = node.as< std::string > ( );
    }
    return result;
}

std::string YAMLExtracted::extractString ( YAML::Node node, std::string key )
{
    assertIsMap ( node, __PRETTY_FUNCTION__ );
    if ( not node [ key ] )
    {
        notifyIncorrectFormat (
                "Node tagged \"" + node.Tag ( )
                + "\" is expected to have a value tagged \"" + key
                + "\" but it does not. An empty string will be read instead." );
        return "";
    }
    return extractString ( node [ key ] );
}

std::vector< std::string > YAMLExtracted::extractSequence ( YAML::Node  node,
                                                            std::string key )
{
    assertIsMap ( node, __PRETTY_FUNCTION__ );
    if ( not node [ key ] )
    {
        notifyIncorrectFormat("Node tagged \"" + node.Tag() + "\" is expected to have a value tagged \"" + key + "\" but it does not. An empty sequence will be read instead.");
        return std::vector< std::string > { };
    } else
    {
        return extractSequence ( node [ key ] );
    }
}

std::vector< std::string > YAMLExtracted::extractSequence ( YAML::Node node )
{
    if ( not node.IsSequence ( ) )
    {
        notifyIncorrectFormat("Node " + node.Tag() + " is expected to be a sequence. It is not a sequence so an empty sequence will be read instead.");
        return std::vector< std::string > { };
    } else
    {
        std::vector< std::string > data;
        for ( std::unsigned_integral auto i = 0U; i < node.size ( ); i++ )
        {
            data.push_back ( extractString ( node [ i ] ) );
        }
        return data;
    }
}

void YAMLExtracted::assertIsMap ( YAML::Node node, std::string checkedBy )
{
    if ( not node.IsMap ( ) )
    {
        throw std::runtime_error (
                "Attempted to extract data with key from node tagged \""
                + node.Tag ( ) + "\" when that node is not even a map. This function was called by " + checkedBy );
    }
}

bool YAMLExtracted::extractOptional ( YAML::Node  node,
                                      std::string key,
                                      bool        defaultValue )
{
    assertIsMap ( node, __PRETTY_FUNCTION__ );
    return node [ key ].as< bool > ( defaultValue );
}

void YAMLExtracted::notifyIncorrectFormat ( std::string message )
{
    application.getCout ( ) << message << delimiters::statementDelimiter;
}

void YAMLExtracted::extractDatum ( YAML::Node node )
{
    std::string theType = extractType ( node );
    if ( theType.empty ( ) )
    {
        notifyIncorrectFormat (
                "Detected error in reading type. Aborting node extraction "
                "attempt." );
        return;
    }

    if ( not manipulators::contains ( types::typesBegin ( ),
                                      types::typesEnd ( ),
                                      theType ) )
    {
        notifyIncorrectFormat (
                "The found type, \"" + theType
                + "\", does not match a type known to prerequisite checker." );
        return;
    }

    getAllTags ( ).push_back ( { } );
    ExtractedItem &item = getAllTags ( ).back ( );
    item.push_back ( Tag { keys::type, theType } );

    if ( theType == types::course )
    {
        courseExtraction ( node, item );
        return;
    } else if ( theType == types::requisites )
    {
        requisitesExtraction ( node, item );
        return;
    } else if ( theType == types::semester )
    {
        semesterExtraction ( node, item );
        return;
    } else if ( theType == types::plan )
    {
        planExtraction ( node, item );
        return;
    } else if ( theType == types::manifest )
    {
        manifestExtraction ( node, item );
        return;
    } else
    {
        throw std::runtime_error ( "This code should be unreachable! Line "
                                   + std::to_string ( __LINE__ ) );
    }
}