#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Requisite;
class Course;
class Plan;

class Requisite
{
public:
    bool                       allowConcurrent = false;
    bool                       allowPreviously = true;
    std::vector< std::string > acceptable;

    bool containsCourse ( Course course );

    friend std::istream &operator>> ( std::istream &, Requisite & );

    std::string str ( );
};

class Course
{
public:
    std::vector< Requisite > requisites;
    std::string              name, desc = "No Name Given.";
    bool                     honors = false, graduate = false;
    double                   hours;

    friend std::istream &operator>> ( std::istream &, Course & );
};

class Plan
{
public:
    std::vector< std::string > incomingAPCredits;
    std::vector< std::string > semesters [ 8 ];
    std::string                name;

    std::string failureMessage ( );

    friend std::istream &operator>> ( std::istream &, Plan & );

    std::string semesterBreakdown ( );

    double *totalHours ( int semester );
};

std::vector< Course > globalCourses;
std::vector< Plan >   globalPlans;

Course *getCourseFromGlobal ( std::string name )
{
    for ( int i = 0; i < globalCourses.size ( ); i++ )
    {
        if ( globalCourses [ i ].name == name )
        {
            return &globalCourses [ i ];
        }
    }
    return nullptr;
}
Plan *getPlanFromGlobal ( std::string name )
{
    for ( int i = 0; i < globalPlans.size ( ); i++ )
    {
        if ( globalPlans [ i ].name == name )
        {
            return &globalPlans [ i ];
        }
    }
    return nullptr;
}

int main ( int const argc, char const *const *const argv )
{
    if ( argc != 3 )
    {
        std::cout << "Usage: " << argv [ 0 ]
                  << " course-manifest-file plan-manifest-file\n";
        return 0;
    } else
    {
        std::ifstream courses ( argv [ 1 ] ), plans ( argv [ 2 ] );
        while ( !courses.eof ( ) )
        {
            std::string line;
            std::getline ( courses, line );
            std::ifstream courseFile ( line );
            while ( !courseFile.eof ( ) )
            {
                Course course;
                courseFile >> course;
            }
        }
        while ( !plans.eof ( ) )
        {
            std::string line;
            std::getline ( plans, line );
            std::ifstream planFile ( line );
            while ( !planFile.eof ( ) )
            {
                Plan plan;
                planFile >> plan;
            }
        }

        for ( Plan plan : globalPlans )
        {
            std::cout << "Plan \"" << plan.name
                      << "\": " << plan.failureMessage ( ) << std::endl;
            std::cout << plan.semesterBreakdown ( ) << std::endl;
        }
        std::cout << "Press enter to view classes by semester and by their "
                     "(human readable) name. Press Ctrl+C to exit.\n";
        std::cin.get ( );
        for ( Plan plan : globalPlans )
        {
            std::cout << "Course Name: ";
            for ( std::string apCourse : plan.incomingAPCredits )
            {
                std::cout << apCourse << ". Semester: AP\n";
            }
            for ( int i = 0; i < 8; i++ )
            {
                for ( std::string course : plan.semesters [ i ] )
                {
                    std::cout << course << ". Semester: " << i << "\n";
                }
            }
        }
    }
}

std::istream &operator>> ( std::istream &stream, Requisite &requisite )
{
    std::string checkword;
    stream >> checkword;
    // std::cout << checkword << std::endl;

    std::string mode;
    stream >> mode;
    if ( mode.find ( "c" ) != std::string::npos )
    {
        requisite.allowConcurrent = true;
    }
    if ( mode.find ( "p" ) != std::string::npos )
    {
        requisite.allowPreviously = true;
    }

    while ( !stream.eof ( ) )
    {
        std::string front, back;
        stream >> front;
        stream >> back;
        requisite.acceptable.push_back ( front + " " + back );
    }

    return stream;
}

std::istream &operator>> ( std::istream &stream, Course &course )
{
    std::string line;
    auto        getline = [ & ] ( ) {
        std::getline ( stream, line );
        return line;
    };

    while ( !stream.eof ( ) && getline ( ) != "course" )
        ;
    while ( !stream.eof ( ) && getline ( ) != "end" )
    {
        std::string       keyword;
        std::stringstream toParse ( line );
        toParse >> keyword;

        if ( keyword == "name" )
        {
            course.name = toParse.str ( ).substr (
                    toParse.str ( ).find ( keyword ) + keyword.size ( ) + 1 );
        } else if ( keyword == "hours" )
        {
            double hours;
            toParse >> hours;
            course.hours = hours;
        } else if ( keyword == "honors" )
        {
            course.honors = true;
        } else if ( keyword == "graduate" )
        {
            course.graduate = true;
        } else if ( keyword == "req" )
        {
            // std::cout << "Found req command in \"" << line << "\"\n";
            toParse = std::stringstream ( line );
            // std::cout << "This is translated into \"" << toParse.str() <<
            // "\"\n";
            Requisite req;
            toParse >> req;
            course.requisites.push_back ( req );
        } else if ( keyword == "desc" )
        {
            course.desc = toParse.str ( ).substr (
                    toParse.str ( ).find ( keyword ) + keyword.size ( ) + 1 );
        }
    }
    globalCourses.push_back ( course );
    return stream;
}

std::istream &operator>> ( std::istream &stream, Plan &plan )
{
    std::string line;
    auto        getline = [ & ] ( ) {
        std::getline ( stream, line );
        return line;
    };

    while ( !stream.eof ( ) && getline ( ) != "plan" )
        ;
    while ( !stream.eof ( ) && getline ( ) != "end" )
    {
        std::string       keyword;
        std::stringstream toParse ( line );
        toParse >> keyword;

        if ( keyword == "name" )
        {
            plan.name = toParse.str ( ).substr (
                    toParse.str ( ).find ( keyword ) + keyword.size ( ) + 1 );
        } else if ( keyword == "semester" )
        {
            std::string semesterSpecified;
            toParse >> semesterSpecified;
            std::vector< std::string > *pSemester;
            if ( semesterSpecified == "AP" )
            {
                // std::cout << "AP Semester!\n";
                pSemester = &plan.incomingAPCredits;
            } else
            {
                std::stringstream converted ( semesterSpecified );
                unsigned          semester;
                converted >> semester;
                pSemester = &plan.semesters [ semester ];
            }

            std::string firstPart, secondPart;
            while ( !toParse.eof ( ) )
            {
                toParse >> firstPart >> secondPart;
                std::string full = firstPart + " " + secondPart;
                // std::cout << "Extracting course " << full << std::endl;
                pSemester->push_back ( full );
            }
        }
    }
    globalPlans.push_back ( plan );
    return stream;
}

bool Requisite::containsCourse ( Course course )
{
    for ( std::string contained : acceptable )
    {
        Course *found = getCourseFromGlobal ( contained );
        if ( found && found->name == course.name )
        {
            return true;
        }
    }
    return false;
}

std::string Plan::failureMessage ( )
{
    bool unsure = false;
    for ( int i = 0; i < 8; i++ )
    {
        for ( std::string currentClass : semesters [ i ] )
        {
            Course *course = getCourseFromGlobal ( currentClass );
            if ( course == nullptr )
            {
                return "Failure: " + currentClass + " is not registered.";
            } else
            {
                for ( Requisite requisite : course->requisites )
                {
                    if ( requisite.acceptable.empty ( ) )
                    {
                        // std::cout << "Error: the one of the requisites for "
                        // + currentClass + " is blank.\n";
                        continue;
                    }
                    bool matched = false;
                    if ( requisite.allowPreviously )
                    {
                        // check AP for requisites
                        for ( std::string ap : incomingAPCredits )
                        {
                            Course *apCourse = getCourseFromGlobal ( ap );
                            if ( !apCourse )
                                unsure = true;
                            else
                            {
                                if ( requisite.containsCourse ( *apCourse ) )
                                {
                                    matched = true;
                                    break;
                                }
                            }
                        }
                        if ( matched )
                            break;
                        // if we did not match, go to previous semesters and
                        // check there
                        else
                            for ( int j = 0; j < i; j++ )
                            {
                                for ( std::string clazz : semesters [ j ] )
                                {
                                    Course *previous =
                                            getCourseFromGlobal ( clazz );
                                    if ( !previous )
                                        unsure = true;
                                    else
                                    {
                                        if ( requisite.containsCourse (
                                                     *previous ) )
                                        {
                                            matched = true;
                                            break;
                                        }
                                    }
                                }
                            }
                    }
                    if ( matched )
                        continue;
                    else if ( requisite.allowConcurrent )
                    {
                        for ( std::string concurrent : semesters [ i ] )
                        {
                            if ( concurrent == currentClass )
                                continue;
                            else
                            {
                                Course *concurrentCourse =
                                        getCourseFromGlobal ( concurrent );
                                if ( !concurrentCourse )
                                    unsure = true;
                                else
                                {
                                    if ( requisite.containsCourse (
                                                 *concurrentCourse ) )
                                    {
                                        matched = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if ( !matched )
                    {
                        return "Failed to match requisite of course "
                             + currentClass + ": " + requisite.str ( );
                    }
                }
            }
        }
    }
    return !unsure ? "No failure."
                   : "No failure found, but some courses are not registered.";
}

std::string Requisite::str ( )
{
    std::stringstream result;
    for ( int i = 0; i < acceptable.size ( ) - 1; i++ )
    {
        result << acceptable [ i ] << " or ";
    }
    result << acceptable.back ( );
    return result.str ( ).substr ( 0, result.str ( ).size ( ) );
}

std::string Plan::semesterBreakdown ( )
{
    std::stringstream result;

    static char const *const semesterNames [] = { "Incoming AP",
                                                  "Fall - 21",
                                                  "Spring - 22",
                                                  "Fall - 22",
                                                  "Spring - 23",
                                                  "Fall - 23",
                                                  "Spring - 24",
                                                  "Fall - 24",
                                                  "Spring - 25" };

    if ( failureMessage ( ).find ( "No failure" ) == std::string::npos )
    {
        result << name << " has errors in it. ";
    }
    result << "The Plan: \n";
    for ( int i = 0; i < 9; i++ )
    {
        result << std::left
               << std::setw ( std::strlen ( semesterNames [ i ] ) + 5 )
               << ( std::string ( semesterNames [ i ] ) ) << "|";
    }
    result << "\n";
    std::vector< std::string > courseNames [ 9 ] = { incomingAPCredits,
                                                     semesters [ 0 ],
                                                     semesters [ 1 ],
                                                     semesters [ 2 ],
                                                     semesters [ 3 ],
                                                     semesters [ 4 ],
                                                     semesters [ 5 ],
                                                     semesters [ 6 ],
                                                     semesters [ 7 ] };
    auto                       checkEmpty        = [ & ] ( ) {
        bool allEmpty = false;
        for ( int i = 0; i < 9; i++ )
        {
            allEmpty |= !courseNames [ i ].empty ( );
        }
        return allEmpty;
    };

    while ( checkEmpty ( ) )
    {
        for ( int i = 0; i < 9; i++ )
        {
            if ( courseNames [ i ].empty ( ) )
            {
                result << std::right
                       << std::setw ( std::strlen ( semesterNames [ i ] ) + 6 )
                       << "|";
            } else
            {
                std::stringstream row;
                row << std::left
                    << std::setw ( std::strlen ( semesterNames [ i ] ) + 1 )
                    << courseNames [ i ].back ( );
                row << std::right << std::setw ( 4 )
                    << ( getCourseFromGlobal ( courseNames [ i ].back ( ) )
                                 ? getCourseFromGlobal (
                                           courseNames [ i ].back ( ) )
                                           ->hours
                                 : 0.0 );
                row << std::setw ( 1 ) << "|";
                result << std::left
                       << std::setw ( std::strlen ( semesterNames [ i ] ) + 6 )
                       << row.str ( );
                courseNames [ i ].pop_back ( );
            }
        }
        result << std::endl;
    }
    char const *creditNames [ 3 ] = { "Total: ", "Honors: ", "Graduate: " };
    for ( int j = 0; j < 3; j++ )
    {
        for ( int i = 0; i < 9; i++ )
        {
            std::stringstream row;
            row << std::left
                << std::setw ( std::strlen ( semesterNames [ i ] ) + 1
                               - ( j == 3 ) )
                << creditNames [ j ];
            double *hours = totalHours ( i - 1 );
            row << std::right << std::setw ( 4 + ( j == 3 ) )
                << ( hours [ j ] );
            delete [] hours;
            row << std::setw ( 1 ) << "|";
            result << std::left
                   << std::setw ( std::strlen ( semesterNames [ i ] ) + 6 )
                   << row.str ( );
        }
        result << "\n";
    }
    double *totalHourCount = totalHours ( -1 );
    for ( int i = 0; i < 8; i++ )
    {
        double *semesterHours = totalHours ( i );
        for ( int j = 0; j < 3; j++ )
        {
            totalHourCount [ j ] += semesterHours [ j ];
        }
        delete [] semesterHours;
    }

    result << std::endl;
    result << "Total Hours: \n";
    result << "General: " << totalHourCount [ 0 ] << "\n";
    result << "Honors: " << totalHourCount [ 1 ] << "\n";
    result << "Graduate: " << totalHourCount [ 2 ] << "\n";
    delete [] totalHourCount;

    return result.str ( );
}

double *Plan::totalHours ( int semester )
{
    double *result =
            new double [ 3 ] { 0.0,
                               0.0,
                               0.0 }; // graduate hours count as honors, so
                                      // graduate + honors doesn't make sense
    if ( semester == -1 )
    {
        for ( std::string courseName : incomingAPCredits )
        {
            if ( getCourseFromGlobal ( courseName ) )
            {
                Course *course = getCourseFromGlobal ( courseName );
                result [ 0 ] += course->hours;
                if ( course->graduate )
                {
                    result [ 1 ] += course->hours; // honors
                    result [ 2 ] += course->hours; // graduate
                } else if ( course->honors )
                {
                    result [ 1 ] += course->hours;
                }
            }
        }
    } else
    {
        for ( std::string courseName : semesters [ semester % 8 ] )
        {
            if ( getCourseFromGlobal ( courseName ) )
            {
                Course *course = getCourseFromGlobal ( courseName );
                result [ 0 ] += course->hours;
                if ( course->graduate )
                {
                    result [ 1 ] += course->hours; // honors
                    result [ 2 ] += course->hours; // graduate
                } else if ( course->honors )
                {
                    result [ 1 ] += course->hours;
                }
            }
        }
    }
    // std::cout << "{ " << result [ 0 ] << ", " << result [ 1 ] << ", " <<
    // result [ 2 ] << " }\n";
    return result;
}