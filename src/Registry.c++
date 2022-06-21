#include "Registry.h++"

#include "Course.h++"
#include "Plan.h++"
#include "Reference.h++"
#include "Requisites.h++"
#include "Semester.h++"
#include "Serial.h++"

#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

void Registry::clear() noexcept
{
    courses.clear();
    requisites.clear();
    semesters.clear();
    plans.clear();
}

void Registry::copy(Registry const &registry) noexcept
{
    for (CoursePointer pcourse : registry.courses)
    {
        courses.push_back(CoursePointer(new Course(*pcourse)));
    }
    for (RequisitesPointer prequisites : registry.requisites)
    {
        requisites.push_back(RequisitesPointer(new Requisites(*prequisites)));
    }
    for (SemesterPointer psemester : registry.semesters)
    {
        semesters.push_back(SemesterPointer(new Semester(*psemester)));
    }
    for (PlanPointer pplan : registry.plans)
    {
        plans.push_back(PlanPointer(new Plan(*pplan)));
    }
}

std::istream &Registry::extract(std::istream &istream)
{
    // we have four keywords that take up one line.
    // courses
    // requisites
    // semesters
    // plans
    //
    // each take an argument which is the file to slurp with the items.
    std::stringstream line;
    std::string temp;
    do
    {
        std::getline(istream, temp);
        line = std::stringstream(temp);
        line >> temp;
        std::string path = line.str().substr(line.str().find(temp) + temp.size() + 1);
        // std::cout << "Will look in path " << path << "\n";
        if (temp == "courses")
        {
            // std::cout << "Slurping courses file...\n";
            std::ifstream coursesFile(path);
            // std::cout << "Courses file opened: " << coursesFile.good ( ) << std::endl;
            while (!coursesFile.eof())
            {
                CoursePointer found = CoursePointer(new Course());
                coursesFile >> *found;
                courses.push_back(found);
                // std::cout << "Found Course " << found->getReference ( ) << "\n";
            }
        }
        else if (temp == "requisites")
        {
            std::ifstream requisitesFile(path);
            while (!requisitesFile.eof())
            {
                RequisitesPointer found = RequisitesPointer(new Requisites());
                requisitesFile >> *found;
                requisites.push_back(found);
            }
        }
        else if (temp == "semesters")
        {
            std::ifstream semestersFile(path);
            while (!semestersFile.eof())
            {
                SemesterPointer found = SemesterPointer(new Semester());
                semestersFile >> *found;
                semesters.push_back(found);
            }
        }
        else if (temp == "plans")
        {
            std::ifstream plansFile(path);
            while (!plansFile.eof())
            {
                PlanPointer found = PlanPointer(new Plan());
                plansFile >> *found;
                plans.push_back(found);
            }
        }
    } while (!istream.eof());
    return istream;
}

void Registry::runTests() const noexcept
{
    for (PlanPointer pplan : plans)
    {
        std::cout << pplan->getPlanMessage(*this) << "\n";
        std::cout << "Press enter to test the next plan.\n";
        std::cin.get();
    }
}