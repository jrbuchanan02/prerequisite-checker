#include <Keywords.h++>
#include <Plan.h++>
#include <Reference.h++>
#include <Registry.h++>

#include <istream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace keywords::keys;
using namespace keywords::types;
using keywords::delimiters::statementDelimiter;

void Plan::extract ( ExtractedItem const &item )
{
    throwOnWrongType ( plan, item );
    // extract our reference.
    Referred::extract ( item );
    // each item that is not "type" and not "ref"
    std::vector< std::string > semesterNames;
    std::for_each ( item.begin ( ), item.end ( ), [ & ] ( auto t ) {
        if ( t.key != type and t.key != ref )
            semesterNames.push_back ( t.key );
    } );
    std::vector< Reference > semesters;
    std::for_each (
            semesterNames.begin ( ),
            semesterNames.end ( ),
            [ & ] ( auto s ) { semesters.push_back ( Reference ( s ) ); } );
    std::for_each ( semesters.begin ( ), semesters.end ( ), [ & ] ( auto r ) {
        this->semesters.emplace ( r, std::vector< Reference > { } );
    } );
    for ( auto pair : this->semesters )
    {
        auto const &semester = pair.first;
        auto       &courses  = this->semesters.at ( semester );

        std::for_each ( item.begin ( ), item.end ( ), [ & ] ( auto t ) {
            if ( t.key == semester )
            {
                courses.push_back ( Reference ( t.val ) );
            }
        } );
    }
}

std::string const generateError ( Plan const      &plan,
                                  Registry const  &registry,
                                  Reference const &course,
                                  Reference const &requisites )
{
    std::string   message = plan.getReference ( ).getName ( );
    CoursePointer pCourse = registry.resolveCourse ( course );
    if ( !pCourse )
    {
        message += " fails because " + course.getName ( )
                 + " cannot be resolved. Is it in the data files?";
    } else
    {
        message += " fails because " + course.getName ( )
                 + " does not meet the requisite " + requisites.getName ( );
    }
    return message;
}

std::string const
        Plan::getPlanMessage ( Registry const &registry ) const noexcept
{
    application.getLog ( ) << "Checking plan " << getReference ( )
                           << statementDelimiter;
    // all of our semesters in chronological (loaded) order
    // because of how std::map works, there is no guarantee that we would
    // iterate through the memebers in order (std::map uses a self-balancing
    // BST, and that would only be in order if the iterator uses an
    // inorder-iteration).
    std::vector< Reference > orderedSemesters = registry.semestersInOrder ( );
    // All of the courses that we have looked at and already verified along with
    // the ones that we are verifying.
    std::vector< std::vector< Reference > > currentAndPriorSemesters;
    // The reference that any non-fufilled course tells us is not met. This
    // reference is to a Requisites.
    Reference                               firstOffendingRequisiteGroup;
    for ( std::integral auto i = 0LLU; i < orderedSemesters.size ( ); i++ )
    {
        application.getLog ( )
                << "\tVerifying semester "
                << orderedSemesters.at ( i ).getName ( ) << statementDelimiter;
        // If our plan has this semester; plans do not necessarily have to have
        // a semester which has already been defined.
        if ( semesters.contains ( orderedSemesters [ i ] ) )
        {
            // This semester is now the one we are verifying.
            currentAndPriorSemesters.push_back (
                    semesters.at ( orderedSemesters [ i ] ) );
            // only check the prerequisites if we should check the semester. No
            // check for nullpointer is made since only defined semesters would
            // show up in the registry
            if ( registry.resolveSemester ( orderedSemesters [ i ] )
                         ->isChecked ( ) )
            {
                application.getLog ( )
                        << "\tThis semester requires verification, checking..."
                        << statementDelimiter;
                // for each course *we* have in this semester
                for ( Reference course :
                      semesters.at ( orderedSemesters [ i ] ) )
                {
                    application.getLog ( ) << "\tChecking requisites for "
                                           << course << statementDelimiter;
                    // resolve the course, if we can
                    CoursePointer pcourse = registry.resolveCourse ( course );
                    if ( !pcourse )
                    {
                        // if the course cannot be resolved, we take it as an
                        // error
                        application.getLog ( )
                                << "\tRealized that " << course
                                << " was never read in. This plan fails!"
                                << statementDelimiter;
                        return generateError ( *this,
                                               registry,
                                               course,
                                               firstOffendingRequisiteGroup );
                    } else
                    {
                        // if the course has unmet requisites, that is an error.
                        application.getLog ( )
                                << "\tChecking if the previous semesters "
                                   "satisfy the requisites..."
                                << statementDelimiter;
                        if ( !pcourse->meetsRequisites (
                                     currentAndPriorSemesters,
                                     registry,
                                     firstOffendingRequisiteGroup ) )
                        {
                            application.getLog ( ) << "\tRequisites did not "
                                                      "pass so this plan fails."
                                                   << statementDelimiter;
                            return generateError (
                                    *this,
                                    registry,
                                    course,
                                    firstOffendingRequisiteGroup );
                        }
                    }
                }
            }
        } else
        {
            application.getLog ( )
                    << "It seems that this plan does not have "
                    << orderedSemesters.at ( i ) << statementDelimiter;
        }
    }
    return getReference ( ).getName ( ) + " passes.";
}