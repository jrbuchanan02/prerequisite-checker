/**
 * @file Extracted.h++
 * @author Joshua Buchanan (joshuarobertbuchanan@gmail.com)
 * @brief Something containing keys and values extracted from a file.
 * @version 1
 * @date 2022-06-24
 *
 * @copyright Copyright (C) 2022. Intellectual property of the author(s) listed above.
 *
 */

#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

class Extracted;
using ExtractedPointer = std::shared_ptr<Extracted>;

struct Tag
{
    std::string key = "";
    std::string val = "";
};

using ExtractedItem = std::vector<Tag>;

class Extracted
{
    std::vector<ExtractedItem> allTags;

protected:
    inline std::vector<ExtractedItem> &getAllTags() noexcept { return allTags; }

public:
    inline std::vector<ExtractedItem> getTagsOfType(std::string type) const
    {
        std::vector<ExtractedItem> tags;
        for (auto extractedItem : allTags)
        {
            bool match = false;
            for (auto tag : extractedItem)
            {
                if (tag.key == "type" and tag.val == type)
                {
                    match = true;
                    break;
                }
            }
            if (match)
            {
                tags.push_back(extractedItem);
            }
        }
        return tags;
    }

    virtual void extractFrom(std::istream &) = 0;
};

class PCFExtracted : public Extracted
{
    void extractSingle(std::istream &i);

public:
    virtual void extractFrom(std::istream &i);
};

class YAMLExtracted : public Extracted
{
public:
    virtual void extractFrom(std::istream &i);
};