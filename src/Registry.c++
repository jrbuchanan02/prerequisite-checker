#include <Course.h++>
#include <Extracted.h++>
#include <Keywords.h++>
#include <Plan.h++>
#include <Reference.h++>
#include <Registry.h++>
#include <Requisites.h++>
#include <Semester.h++>
#include <Serial.h++>
#include <main.h++>

#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

using keywords::delimiters::statementDelimiter;
using namespace keywords::types;

void Registry::clear ( ) noexcept
{
    courses.clear ( );
    requisites.clear ( );
    semesters.clear ( );
    plans.clear ( );
}

void Registry::copy ( Registry const &registry ) noexcept
{
    for ( CoursePointer pcourse : registry.courses )
    {
        courses.push_back ( CoursePointer ( new Course ( *pcourse ) ) );
    }
    for ( RequisitesPointer prequisites : registry.requisites )
    {
        requisites.push_back (
                RequisitesPointer ( new Requisites ( *prequisites ) ) );
    }
    for ( SemesterPointer psemester : registry.semesters )
    {
        semesters.push_back ( SemesterPointer ( new Semester ( *psemester ) ) );
    }
    for ( PlanPointer pplan : registry.plans )
    {
        plans.push_back ( PlanPointer ( new Plan ( *pplan ) ) );
    }
}

void Registry::parse ( Extracted const &extracted )
{
    application.getLog ( ) << "Parsing begin." << statementDelimiter;
    application.getLog ( ) << "Parsing courses..." << statementDelimiter;
    std::integral auto counter = 0;
    for ( ExtractedItem item : extracted.getTagsOfType ( course ) )
    {
        application.getLog ( )
                << "Extracting course " << ++counter << statementDelimiter;
        CoursePointer pointer = CoursePointer ( new Course ( ) );
        pointer->extract ( item );
        courses.push_back ( pointer );
    }
    application.getLog ( ) << "Parsing requisites..." << statementDelimiter;
    for ( ExtractedItem item : extracted.getTagsOfType ( ::requisites ) )
    {
        RequisitesPointer pointer = RequisitesPointer ( new Requisites ( ) );
        pointer->extract ( item );
        requisites.push_back ( pointer );
    }
    application.getLog ( ) << "Parsing semesters..." << statementDelimiter;
    for ( ExtractedItem item : extracted.getTagsOfType ( semester ) )
    {
        SemesterPointer pointer = SemesterPointer ( new Semester ( ) );
        pointer->extract ( item );
        semesters.push_back ( pointer );
    }
    application.getLog ( ) << "Parsing plans..." << statementDelimiter;
    for ( ExtractedItem item : extracted.getTagsOfType ( plan ) )
    {
        PlanPointer pointer = PlanPointer ( new Plan ( ) );
        pointer->extract ( item );
        plans.push_back ( pointer );
    }
    application.getLog ( ) << "Parsing complete." << statementDelimiter;
}

void Registry::runTests ( ) const noexcept
{
    for ( PlanPointer pplan : plans )
    {
        application.getCout ( )
                << pplan->getPlanMessage ( *this ) << statementDelimiter;
        application.getCout ( )
                << "Press enter to test the next plan." << statementDelimiter;
        application.getCin ( ).get ( );
    }
}

std::vector< Reference > const Registry::semestersInOrder ( ) const noexcept
{
    std::vector< Reference > output;
    for ( SemesterPointer const &psemester : semesters )
    {
        if ( psemester )
        {
            output.push_back (
                    ( ( Semester const ) *psemester ).getReference ( ) );
        }
    }
    return output;
}

CoursePointer
        Registry::resolveCourse ( Reference const &reference ) const noexcept
{
    for ( CoursePointer const &pcourse : courses )
    {
        if ( ( ( Course const ) *pcourse ).getReference ( ) == reference )
        {
            return pcourse;
        }
    }
    return nullptr;
}

std::vector< Reference > const Registry::knownCourses ( ) const noexcept
{
    std::vector< Reference > known ( courses.size ( ) );
    for ( auto i = 0LLU; i < courses.size ( ); i++ )
    {
        known [ i ] = ( ( Course const ) *courses [ i ] ).getReference ( );
    }
    return known;
}

RequisitesPointer Registry::resolveRequisites (
        Reference const &reference ) const noexcept
{
    application.getLog ( ) << "Received request to resolve reference \""
                           << reference << "\"" << statementDelimiter;
    for ( RequisitesPointer const &pRequisites : requisites )
    {
        auto requisiteReference =
                ( ( Requisites const ) *pRequisites ).getReference ( );
        application.getLog ( )
                << "Checking if \"" << requisiteReference << "\" is \""
                << reference << "\"" << statementDelimiter;
        if ( requisiteReference == reference )
        {
            return pRequisites;
        }
    }
    return nullptr;
}

SemesterPointer
        Registry::resolveSemester ( Reference const &reference ) const noexcept
{
    for ( SemesterPointer const &psemester : semesters )
    {
        if ( ( ( Semester const ) *psemester ).getReference ( ) == reference )
        {
            return psemester;
        }
    }
    return nullptr;
}

PlanPointer Registry::resolvePlan ( Reference const &reference ) const noexcept
{
    for ( PlanPointer const &pplan : plans )
    {
        if ( ( ( Plan const ) *pplan ).getReference ( ) == reference )
        {
            return pplan;
        }
    }
    return nullptr;
}

Registry &Registry::operator= ( Registry const &registry ) noexcept
{
    this->clear ( );
    this->copy ( registry );
    return *this;
}

Registry &Registry::operator= ( Registry &&registry ) noexcept
{
    this->operator= ( ( Registry & ) registry );
    registry.clear ( );
    return *this;
}