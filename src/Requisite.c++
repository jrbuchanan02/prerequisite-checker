#include <Requisite.h++>
#include <Flagged.h++>
#include <Reference.h++>

#include <istream>
#include <sstream>
#include <string>

std::istream &Requisite::extract(std::istream &istream)
{
    // std::stringstream line;
    std::string temp[4] = {"", "", "", ""};
    // we are expecting pre|con [pre|con] WORD1 WORD2
    // we cannot for certain grab all four words, so we should
    // grab three and check if the second one is either "pre" or
    // "con" before grabbing the fourth
    for (int i = 0; i < 3; i++)
        istream >> temp[i];
    if (temp[1] == "pre" || temp[1] == "con")
    {
        istream >> temp[3];
    }
    else
    {
        temp[3] = temp[2];
        temp[2] = temp[1];
        temp[1] = temp[0];
    }

    // now fill in the information
    addFlag(temp[0]);
    addFlag(temp[1]);
    std::stringstream line(temp[2] + " " + temp[3]);
    line >> course;
    return istream;
}

bool const Requisite::allowPreviously() const noexcept { return hasFlag("pre"); }
bool const Requisite::allowConcurrent() const noexcept { return hasFlag("con"); }

Reference const &Requisite::getCourse() const noexcept { return course; }