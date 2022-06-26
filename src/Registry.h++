#pragma once

#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

class Registry;

#include <Course.h++>
#include <Plan.h++>
#include <Reference.h++>
#include <Requisites.h++>
#include <Semester.h++>
#include <Serial.h++>
#include <main.h++>

class Registry
{
    std::vector< CoursePointer >     courses;
    std::vector< RequisitesPointer > requisites;
    std::vector< SemesterPointer >   semesters;
    std::vector< PlanPointer >       plans;
    std::string                      file;

    void clear ( ) noexcept;
    void copy ( Registry const &registry ) noexcept;
public:
    Registry ( ) noexcept = default;
    Registry ( Registry const &registry ) { copy ( registry ); }
    Registry ( Registry &&registry )
    {
        copy ( registry );
        registry.clear ( );
    }

    virtual ~Registry ( ) { clear ( ); }

    std::vector< Reference > const semestersInOrder ( ) const noexcept;

    CoursePointer resolveCourse ( Reference const &reference ) const noexcept;

    std::vector< Reference > const knownCourses ( ) const noexcept;

    RequisitesPointer
            resolveRequisites ( Reference const &reference ) const noexcept;

    SemesterPointer
            resolveSemester ( Reference const &reference ) const noexcept;

    PlanPointer resolvePlan ( Reference const &reference ) const noexcept;

    Registry &operator= ( Registry const &registry ) noexcept;

    Registry &operator= ( Registry &&registry ) noexcept;

    void runTests ( ) const noexcept;

    void parse ( Extracted const & );
};
