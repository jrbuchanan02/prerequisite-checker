#include <Extracted.h++>
#include <Semester.h++>

#include <istream>
#include <sstream>
#include <string>
#include <vector>

void Semester::extract(ExtractedItem const &extracted)
{
    // find the tag "type" and ensure that it matches "semester".
    if (not Serial::isCorrectType("semester", extracted))
    {
        throw;
    }
    // Extract our reference.
    // TODO!

    // Semester only cares about being tagged "unchecked", if so,
    // we add that flag.
    auto unchecked = std::find_if(extracted.begin(), extracted.end(), [](auto t)
                                  { return t.key == "tagged" and t.val == "unchecked"; });
    if (unchecked != extracted.end())
    {
        addFlag("unchecked");
    }
}

bool const Semester::isChecked() const noexcept
{
    return not hasFlag("unchecked");
}