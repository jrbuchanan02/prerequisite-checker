#include <Course.h++>
#include <Registry.h++>
#include <main.h++>

#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

Course::Course(std::vector<std::string> const &flags, Reference const &reference, double const &hours, std::vector<Reference> const &requisites, std::string const &name, std::string const &desc) noexcept : Flagged(flags), Referred(reference), hours(hours), requisites(requisites), name(name), desc(desc) {}

double const &Course::getHours() const noexcept { return hours; }

std::string const &Course::getName() const noexcept { return name; }

std::string const &Course::getDesc() const noexcept { return desc; }

void Course::extract(ExtractedItem const &item)
{
    application.getLog() << "Extracting course at 0x" << (void *)this << "\n";
    if (not isCorrectType("course", item))
    {
        throw;
    }

    auto findAll = [&](std::string keyword)
    {
        ExtractedItem result;
        for (Tag tag : item)
        {
            if (tag.key == keyword)
            {
                result.push_back(tag);
            }
        }
        return result;
    };

    application.getLog() << "Here.\n";
    // grab reference
    getReference().extract(item);
    application.getLog() << "Here.\n";
    // find all tags which are reqs
    ExtractedItem reqs = findAll("reqs");
    application.getLog() << "Here.\n";
    std::for_each(reqs.begin(), reqs.end(), [&](auto t)
                  { requisites.push_back(t.val); });
    application.getLog() << "Here.\n";
    // find all tags which are hours
    ExtractedItem hours = findAll("hours");
    application.getLog() << "Here.\n";
    std::for_each(hours.begin(), hours.end(), [&](auto t)
                  { double found = 0.0;
        std::stringstream { t.val } >> found; if ( found > 0) { this->hours = found;} });
    application.getLog() << "Here.\n";
    // find all tags which are name
    ExtractedItem names = findAll("name");
    application.getLog() << "Here.\n";
    std::for_each(names.begin(), names.end(), [&](auto t)
                  { name = name + " " + t.val; });
    application.getLog() << "Here.\n";
    // find all tags which are desc
    ExtractedItem descs = findAll("desc");
    application.getLog() << "Here.\n";
    std::for_each(descs.begin(), descs.end(), [&](auto t)
                  { desc = desc + " " + t.val; });
    application.getLog() << "Here.\n";
    // find all tags which are tagged
    ExtractedItem flags = findAll("tagged");
    application.getLog() << "Here.\n";
    std::for_each(flags.begin(), flags.end(), [&](auto t)
                  { addFlag(t.val); });
    application.getLog() << "Here.\n";
}

std::vector<RequisitesPointer> const Course::resolveRequisites(Registry const &withRegistry) noexcept
{
    std::vector<RequisitesPointer> output;
    for (Reference requisites : this->requisites)
    {
        output.push_back(withRegistry.resolveRequisites(requisites));
    }
    return output;
}

bool const Course::meetsRequisites(std::vector<std::vector<Reference>> const &courses, Registry const &registry, Reference &offending)
{
    // this vector is orientated in semesters going from beginning (courses[0]) to end (courses.back())

    // if there are no courses, we do not meet the prerequisites
    for (Reference requisiteGroup : requisites)
    {
        RequisitesPointer prequisites = registry.resolveRequisites(requisiteGroup);
        if (prequisites)
        {
            bool foundMatch = false;

            for (auto semester : courses)
            {
                for (Reference course : semester)
                {
                    CoursePointer pcourse = registry.resolveCourse(course);
                    if (pcourse)
                    {
                        bool matchState = prequisites->meetsRequisite(*pcourse);
                        if (matchState)
                        {
                            application.getLog() << course << " matches " << getReference() << "\n";
                        }
                        foundMatch |= matchState;
                    }
                }
            }
            offending = requisiteGroup;
            if (!foundMatch)
            {
                return false;
            }
        }
    }

    return true; // if we are here, either all requisites were met, or there are no requisites
}

bool const Course::hoursHaveAttribute(std::string const &attribute) const noexcept
{
    return hasFlag(attribute);
}