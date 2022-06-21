#pragma once

#include "Course.h++"
#include "Plan.h++"
#include "Reference.h++"
#include "Requisites.h++"
#include "Semester.h++"
#include "Serial.h++"

#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

class Registry : public Serial
{
    std::vector<CoursePointer> courses;
    std::vector<RequisitesPointer> requisites;
    std::vector<SemesterPointer> semesters;
    std::vector<PlanPointer> plans;
    std::string file;
    
    void clear() noexcept;
    void copy(Registry const &registry) noexcept;

protected:
    virtual std::istream &extract(std::istream &istream) override;

public:
    Registry() noexcept = default;
    Registry(Registry const &registry) : Serial(registry)
    {
        copy(registry);
    }
    Registry(Registry &&registry) : Serial(registry)
    {
        copy(registry);
        registry.clear();
    }

    virtual ~Registry()
    {
        clear();
    }

    std::vector<Reference> const semestersInOrder() const noexcept
    {
        std::vector<Reference> output;
        for (SemesterPointer const &psemester : semesters)
        {
            if (psemester)
            {
                output.push_back(psemester->getReference());
            }
        }
        return output;
    }

    CoursePointer resolveCourse(Reference const &reference) const noexcept
    {
        for (CoursePointer const &pcourse : courses)
        {
            if (pcourse->getReference() == reference)
            {
                return pcourse;
            }
        }
        return nullptr;
    }

    std::vector<Reference> const knownCourses() const noexcept
    {
        std::vector<Reference> known(courses.size());
        for (auto i = 0LLU; i < courses.size(); i++)
        {
            known[i] = courses[i]->getReference();
        }
        return known;
    }

    RequisitesPointer resolveRequisites(Reference const &reference) const noexcept
    {
        for (RequisitesPointer const &prequisites : requisites)
        {
            if (prequisites->getReference() == reference)
            {
                return prequisites;
            }
        }
        return nullptr;
    }

    SemesterPointer resolveSemester(Reference const &reference) const noexcept
    {
        for (SemesterPointer const &psemester : semesters)
        {
            if (psemester->getReference() == reference)
            {
                return psemester;
            }
        }
        return nullptr;
    }

    PlanPointer resolvePlan(Reference const &reference) const noexcept
    {
        for (PlanPointer const &pplan : plans)
        {
            if (pplan->getReference() == reference)
            {
                return pplan;
            }
        }
        return nullptr;
    }

    Registry &operator=(Registry const &registry) noexcept
    {
        this->Serial::operator=(registry);
        this->clear();
        this->copy(registry);
        return *this;
    }

    Registry &operator=(Registry &&registry) noexcept
    {
        this->operator=((Registry &)registry);
        registry.clear();
        return *this;
    }

    void runTests() const noexcept;
};
