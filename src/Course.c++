#include <Course.h++>
#include <Keywords.h++>
#include <Registry.h++>
#include <main.h++>

#include <functional>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

using namespace keywords;
using namespace keywords::keys;
using namespace keywords::delimiters;

void Course::initializeActions ( )
{
    using namespace std::placeholders;
    auto bind = [ & ] ( std::function< void ( Course *, Tag ) > fn ) {
        return std::bind ( fn, this, _1 );
    };
    actions.push_back ( { ::reqs, bind ( &Course::parseSingleReqs ) } );
    actions.push_back ( { ::hours, bind ( &Course::parseSingleHour ) } );
    actions.push_back ( { ::name, bind ( &Course::parseSingleName ) } );
    actions.push_back ( { ::desc, bind ( &Course::parseSingleDesc ) } );
    actions.push_back ( { ::tagged, bind ( &Course::parseSingleFlag ) } );
}

Course::Course ( ) noexcept { initializeActions ( ); }

Course::Course ( std::vector< std::string > const &flags,
                 Reference const                  &reference,
                 double const                     &hours,
                 std::vector< Reference > const   &requisites,
                 std::string const                &name,
                 std::string const                &desc ) noexcept :
        Flagged ( flags ),
        Referred ( reference ), hours ( hours ), requisites ( requisites ),
        name ( name ), desc ( desc )
{
    initializeActions ( );
}

double const &Course::getHours ( ) const noexcept { return hours; }

std::string const &Course::getName ( ) const noexcept { return name; }

std::string const &Course::getDesc ( ) const noexcept { return desc; }

void Course::extract ( ExtractedItem const &item )
{
    application.getLog ( ) << "Extracting course at 0x" << ( void * ) this
                           << statementDelimiter;
    throwOnWrongType ( types::course, item );
    // grab reference
    Referred::extract ( item );

    auto doActionFor = [ & ] ( std::string tag ) {
        CourseParseAction action = chooseAction ( tag );
        for ( Tag t : filterForTagType ( item, tag ) ) { action.action ( t ); }
    };

    doActionFor ( ::hours );
    doActionFor ( ::name );
    doActionFor ( ::desc );
    doActionFor ( reqs );
    doActionFor ( tagged );
}

std::vector< RequisitesPointer > const
        Course::resolveRequisites ( Registry const &withRegistry ) noexcept
{
    std::vector< RequisitesPointer > output;
    for ( Reference requisites : this->requisites )
    {
        output.push_back ( withRegistry.resolveRequisites ( requisites ) );
    }
    return output;
}

bool const Course::meetsRequisites (
        std::vector< std::vector< Reference > > const &courses,
        Registry const                                &registry,
        Reference                                     &offending )
{
    application.getLog ( ) << "\t\tChecking requisites..."
                           << statementDelimiter;

    // if there are no courses, we do not meet the prerequisites
    for ( Reference requisiteGroup : requisites )
    {
        application.getLog ( )
                << "\t\tAsserting that requisites " << requisiteGroup
                << " passes." << statementDelimiter;
        RequisitesPointer prequisites =
                registry.resolveRequisites ( requisiteGroup );
        if ( prequisites )
        {
            bool foundMatch = false;

            for ( auto semester : courses )
            {
                for ( Reference course : semester )
                {
                    application.getLog ( )
                            << "\t\tChecking if previous / concurrent course "
                            << course << " matches." << statementDelimiter;
                    CoursePointer pcourse = registry.resolveCourse ( course );
                    if ( pcourse )
                    {
                        bool matchState =
                                prequisites->meetsRequisite ( *pcourse );
                        if ( matchState )
                        {
                            application.getLog ( )
                                    << course << " matches " << getReference ( )
                                    << statementDelimiter;
                        }
                        foundMatch |= matchState;
                    }
                }
            }
            offending = requisiteGroup;
            if ( !foundMatch )
            {
                return false;
            }
        } else
        {
            application.getLog ( ) << "\t\tCould not find this requisite "
                                      "group. So the data may be garbage?"
                                   << statementDelimiter;
            offending = requisiteGroup;
            return false;
        }
    }

    return true; // if we are here, either all requisites were met, or there are
                 // no requisites
}

bool const Course::hoursHaveAttribute (
        std::string const &attribute ) const noexcept
{
    return hasFlag ( attribute );
}

void Course::parseSingleReqs ( Tag tag ) { requisites.push_back ( tag.val ); }

void Course::parseSingleHour ( Tag tag )
{
    double found = 0;
    std::stringstream { tag.val } >> found;
    if ( found > 0 )
    {
        hours = found;
    }
}

void Course::parseSingleName ( Tag tag ) { name = name + space + tag.val; }

void Course::parseSingleDesc ( Tag tag ) { desc = desc + space + tag.val; }

void Course::parseSingleFlag ( Tag tag ) { addFlag ( tag.val ); }

Course::CourseParseAction Course::chooseAction ( std::string tag )
{
    for ( auto action : actions )
    {
        if ( action.tagName == tag )
        {
            return action;
        }
    }
    throw std::runtime_error ( "Unknown tag: " + tag );
}