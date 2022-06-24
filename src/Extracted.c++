/**
 * @file Extracted.c++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Implements an Extracted.
 * @note YAML-cpp lies within the repository just for this file.
 * @version 1
 * @date 2022-06-24
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 *
 */

#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <Extracted.h++>
#include <main.h++>

#include <yaml-cpp/yaml.h>

static std::string const typeKey = "type";
static std::string const refKey = "ref";
static std::string const nameKey = "name";
static std::string const descKey = "desc";
static std::string const hoursKey = "hours";
static std::string const reqsKey = "reqs";
static std::string const taggedKey = "tagged";
static std::string const commentKey = "#";
static std::string const reqKey = "req";
static std::string const coursesKey = "courses";
static std::string const requisitesKey = "requisites";
static std::string const plansKey = "plans";
static std::string const semestersKey = "semesters";

static std::string const beginPrefix = "";
static std::string const endPrefix = "end";

static std::string const course = "course";
static std::string const requisites = "reqs";
static std::string const semester = "semester";
static std::string const plan = "plan";
static std::string const manifest = "manifest";

static std::string const takenPreviously = "pre";
static std::string const takenConcurrent = "con";

static std::string const courseDelimiter = "or";
static std::string const planReferenceEnd = ":";
static std::string const planCourseDelimiter = "and";

std::string beginOf(std::string name)
{
    return beginPrefix + name;
}

std::string endOf(std::string name)
{
    return endPrefix + name;
}

static std::vector<std::string> const types = {
    course,
    requisites,
    semester,
    plan,
    manifest,
};

// extracts a single element from a single file.
void PCFExtracted::extractSingle(std::istream &i)
{
    std::vector<std::string> possibleBegins = types;
    std::vector<std::string> possibleEnds = types;
    std::for_each(possibleBegins.begin(), possibleBegins.end(), [](std::string &s)
                  { s = beginOf(s); });
    std::for_each(possibleEnds.begin(), possibleEnds.end(), [](std::string &s)
                  { s = endOf(s); });

    std::string line = "";

    auto getLine = [&]()
    {
        std::getline(i, line);
    };

    bool foundSomething = false;
    std::string foundKey = "";
    while (not foundSomething and not i.eof())
    {
        getLine();
        std::for_each(possibleBegins.begin(), possibleBegins.end(), [&](auto s)
                      { if ( line.starts_with(s) and foundKey == "") {foundSomething = true; foundKey = s;} });
    }
    if (not foundSomething)
    {
        application.getLog() << "Did not find tag.\n";
        return;
    }

    while (not i.eof())
    {
        application.getLog() << "Line marks beginning of a " << foundKey << "\n";
        getAllTags().push_back({});
        ExtractedItem &item = getAllTags().back();
        item.push_back(Tag{typeKey, foundKey});
        // if we found Requisites, there may be some extra processing...
        bool foundRequisites = foundKey == beginOf(requisites);
        bool foundPlan = foundKey == beginOf(plan);
        bool foundEnd = false;
        std::integral auto currentRequisiteID = 0;
        while (not foundEnd and not i.eof())
        {
            getLine();
            std::stringstream lineParser{line};
            std::string keyword = "";
            lineParser >> keyword;
            application.getLog() << "This line's keyword is \"" << keyword << "\"\n";
            if (keyword.starts_with(commentKey))
            {
                continue;
            }
            std::string rest = "";
            rest = line.substr(line.find(keyword) + keyword.size());
            while (rest.starts_with(" "))
            {
                rest = rest.substr(1);
            }
            while (rest.ends_with(" ") or rest.ends_with("\n"))
            {
                rest = rest.substr(0, rest.size() - 1);
            }
            if (keyword == endOf(foundKey))
            {
                foundEnd = true;
            }
            if (foundRequisites and keyword == reqKey)
            {
                // extra processing here.
                // we need different keys for when a requisite must
                // be taken previously, concurrently, either and if
                // multiple requisites satisfy the same category. For
                // example, Calculus I and Honors Calculus I may appear
                // on the same line and both satisfy the requisite for
                // Calculus II. So, we keep an integral ID of each requisite.
                // if the counter is the same, the courses both satisfy the same
                // set of requirements.
                std::string id = std::to_string(currentRequisiteID++);
                std::string key = keyword + " " + id;
                std::stringstream restParser{rest};
                while (restParser)
                {
                    std::string requisite = "";
                    while (requisite.empty() or (not requisite.ends_with(courseDelimiter) and restParser))
                    {
                        restParser >> requisite;
                    }

                    std::stringstream requisiteParser{requisite};
                    std::string nextTwo[2]{"", ""};
                    requisiteParser >> nextTwo[0] >> nextTwo[1];
                    if (nextTwo[0] == takenPreviously or nextTwo[1] == takenPreviously)
                    {
                        key += " " + takenPreviously;
                    }
                    if (nextTwo[0] == takenConcurrent or nextTwo[1] == takenConcurrent)
                    {
                        key += " " + takenConcurrent;
                    }

                    if (requisite.find(takenPreviously) != std::string::npos)
                    {
                        application.getLog() << "This requisite may be taken previously.\n";
                        std::unsigned_integral auto position = requisite.find_last_of(takenPreviously);
                        requisite = requisite.substr(position + takenPreviously.size());
                    }
                    if (requisite.find(takenConcurrent) != std::string::npos)
                    {
                        application.getLog() << "This requisite may be taken concurrent.\n";
                        std::unsigned_integral auto position = requisite.find_last_of(takenConcurrent);
                        requisite = requisite.substr(position + takenConcurrent.size());
                    }

                    item.push_back(Tag{key, requisite});
                }

                continue;
            }
            if (foundPlan and keyword == semester)
            {
                // take rest from beginning to the ending
                // then split each other course reference on the
                // line and add it as a tag.
                std::string key = rest.substr(0, rest.find(planReferenceEnd));
                application.getLog() << "Key is now " << key << "\n";
                std::stringstream restParser{rest.substr(rest.find(planReferenceEnd) + planReferenceEnd.size())};
                application.getLog() << "Parsing all information from " << restParser.str() << "\n";
                while (restParser)
                {
                    std::string val = "";
                    while (restParser and not val.ends_with(courseDelimiter) and not val.ends_with(planCourseDelimiter))
                    {
                        std::string temp = "";
                        restParser >> temp;
                        val += " " + temp;
                        application.getLog() << "value is now " << val << "\n";
                    }
                    if (val.ends_with(courseDelimiter))
                    {
                        val = val.substr(0, val.find(courseDelimiter));
                    }
                    if (val.ends_with(planCourseDelimiter) and planCourseDelimiter != courseDelimiter)
                    {
                        val = val.substr(0, val.find(planCourseDelimiter));
                    }
                    item.push_back(Tag{key, val});
                }
            }
            application.getLog() << "Added key/value pair " << keyword << " and " << rest << "\n";
            if (not foundEnd and not keyword.empty() and not rest.empty())
            {
                item.push_back({keyword, rest});
            }
        }
        if (getAllTags().back().size() < 2 and getAllTags().back().front().val != semester)
        {
            application.getLog() << "Found empty " << getAllTags().back().front().val << ".\n";
            getAllTags().pop_back();
        }
        if (getAllTags().back().front().key == "type" and getAllTags().back().front().val == "manifest")
        {
            application.getLog() << "\tThis data is a manifest, opening all relavent files...\n";
            for (auto tag : getAllTags().back())
            {
                if (tag.key == typeKey)
                {
                    continue;
                }
                application.getLog() << "\tStatus of file " << tag.val << ": ";
                std::ifstream file{tag.val};
                if (file and not file.bad())
                {
                    application.getLog() << "Reading...\n";
                    extractSingle(file);
                }
                else
                {
                    application.getLog() << "Failed to open.\n";
                }
            }
        }
    }
}

void PCFExtracted::extractFrom(std::istream &i)
{
    application.getLog() << "Extracting file(s)...\n";
    while (not i.eof())
    {
        application.getLog() << "\tFound Tag?\n";
        extractSingle(i);
    }
    application.getLog() << "Extraction finished.\n";
}

void YAMLExtracted::extractFrom(std::istream &i)
{
    auto nodes = YAML::LoadAll(i);
    for (auto node : nodes)
    {
        if (not node.IsSequence())
        {
            application.getCout() << "Wrong YAML Format.\n";
            return;
        }
        for (auto item = node.begin(); item != node.end(); item++)
        {
            getAllTags().push_back({});
            ExtractedItem &extracted = getAllTags().back();
            std::string keyword = (*item)["type"].as<std::string>();
            bool foundRequisites = keyword == beginOf(requisites);
            bool foundPlan = keyword == beginOf(plan);

            for (auto tags = item->begin(); tags != item->end(); tags++)
            {
                // processing.
            }
        }
    }
}