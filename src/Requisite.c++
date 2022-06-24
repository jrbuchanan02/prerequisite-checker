#include <Flagged.h++>
#include <Reference.h++>
#include <Requisite.h++>

#include <istream>
#include <sstream>
#include <string>

bool const Requisite::allowPreviously() const noexcept { return hasFlag("pre"); }
bool const Requisite::allowConcurrent() const noexcept { return hasFlag("con"); }

Reference const &Requisite::getCourse() const noexcept { return course; }

void Requisite::extract(ExtractedItem const &extracted)
{
    // find the element at counter.
    if (counter < 0 or counter >= extracted.size())
    {
        throw;
    }
    Tag information = extracted.at(counter);
    if (information.key.find("pre") != std::string::npos)
    {
        addFlag("pre");
    }
    if (information.key.find("con") != std::string::npos)
    {
        addFlag("con");
    }

    course = Reference(information.val);
}