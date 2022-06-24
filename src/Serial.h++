#pragma once

#include <istream>
#include <memory>

#include <Extracted.h++>

class Serial
{
protected:
    static inline bool isCorrectType(std::string type, ExtractedItem const &item)
    {
        auto position = std::find_if(item.begin(), item.end(), [&](auto t)
                                     { return t.key == "type" and t.val == type; });
        return position != item.end();
    }

public:
    Serial() noexcept = default;
    Serial(Serial const &) noexcept = default;
    Serial(Serial &&) noexcept = default;
    virtual ~Serial() = default;

    Serial &operator=(Serial const &) noexcept = default;
    Serial &operator=(Serial &&) noexcept = default;

    virtual void extract(ExtractedItem const &) = 0;
};