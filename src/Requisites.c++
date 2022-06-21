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

std::istream &Requisites::extract(std::istream &istream)
{
    // requisites begins with a keyword: "reqs"
    std::stringstream line;
    std::string temp;
    do
    {
        std::getline(istream, temp);
        line = std::stringstream(temp);
        line >> temp;
    } while (temp != "reqs");
    // now, we are looking for one of two keywords: "ref" or "req" and we will end when we find "endreqs"
    do
    {
        std::getline(istream, temp);
        line = std::stringstream(temp);
        line >> temp;
        if (temp == "ref")
        {
            grabReference(line);
        }
        else if (temp == "req")
        {
            while (!line.eof())
            {
                Requisite requisite;
                line >> requisite;
                requisites.push_back(requisite);
            }
        }
    } while (temp != "endreqs");
    return istream;
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