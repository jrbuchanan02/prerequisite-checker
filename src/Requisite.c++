#include <Flagged.h++>
#include <Keywords.h++>
#include <Reference.h++>
#include <Requisite.h++>

#include <istream>
#include <sstream>
#include <string>

using keywords::tags::takenConcurrent;
using keywords::tags::takenPreviously;

bool const Requisite::allowPreviously ( ) const noexcept
{
    return hasFlag ( takenPreviously );
}
bool const Requisite::allowConcurrent ( ) const noexcept
{
    return hasFlag ( takenConcurrent );
}

Reference const &Requisite::getCourse ( ) const noexcept { return course; }

void Requisite::extract ( ExtractedItem const &extracted )
{
    // find the element at counter.
    if ( counter < 0 or counter >= extracted.size ( ) )
    {
        throw;
    }
    Tag information = extracted.at ( counter );
    if ( information.key.find ( takenPreviously ) != std::string::npos )
    {
        addFlag ( takenPreviously );
    }
    if ( information.key.find ( takenConcurrent ) != std::string::npos )
    {
        addFlag ( takenConcurrent );
    }

    course = Reference ( information.val );
}