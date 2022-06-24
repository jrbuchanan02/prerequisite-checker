#include <Course.h++>
#include <Plan.h++>
#include <Referred.h++>
#include <Registry.h++>
#include <Requisite.h++>
#include <Requisites.h++>

#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

void Requisites::extract(ExtractedItem const &extracted)
{
    if (not isCorrectType("reqs", extracted))
    {
        throw;
    }
    // extract everything...
    std::intmax_t counter = 0;
    auto hasAny = [&]()
    {
        auto first = std::find_if(extracted.begin(), extracted.end(), [&](auto t)
                                  { return t.key.starts_with("req " + std::to_string(counter)); });
        return first != extracted.end();
    };
    for (; hasAny(); counter++)
    {
        // find all requisites matching this group.
        ExtractedItem filtered;
        for (Tag tag : extracted)
        {
            if (tag.key.starts_with("req " + std::to_string(counter)))
            {
                filtered.push_back(tag);
            }
        }
        for (std::intmax_t i = 0; Tag tag : filtered)
        {
            Requisite found;
            found.counter = i++;
            found.extract(filtered);
            requisites.push_back(found);
        }
    }
}

bool const Requisites::meetsRequisite(Course const &course) const noexcept
{

    for (Requisite requisite : requisites)
    {
        std::cout << "Checking if " << requisite.getCourse() << " is " << course.getReference() << "\n";
        if (requisite.getCourse() == course.getReference())
        {
            std::cout << "Found that " << requisite.getCourse() << " is " << course.getReference() << "\n";
            return true;
        }
    }
    std::cout << "Could not match " << course.getReference() << "\n";
    return false;
}

std::vector<Requisite> const &Requisites::getRequisites() const noexcept
{
    return requisites;
}