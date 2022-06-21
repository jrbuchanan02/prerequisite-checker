#pragma once

#include "Serial.h++"

#include <istream>
#include <string>

class Reference : public Serial
{
    std::string contents = "";

protected:
    virtual std::istream &extract(std::istream &istream) override
    {
        std::getline(istream, contents);
        while (contents.starts_with(" "))
        {
            contents = contents.substr(1);
        }
        return istream;
    }

public:
    Reference() noexcept = default;
    Reference(Reference const &) noexcept = default;
    Reference(Reference &&) noexcept = default;
    Reference(std::string const &contents) noexcept
    {
        this->contents = contents;
    }
    Reference(std::istream &istream) : Serial(istream) {}
    virtual ~Reference() = default;

    std::string const getName() const noexcept { return contents; }

    Reference &operator=(Reference const &) noexcept = default;
    Reference &operator=(Reference &&) noexcept = default;

    auto operator<=>(Reference const &that) const noexcept
    {
        return contents <=> that.contents;
    }

    bool const operator==(Reference const &that) const noexcept
    {
        return !(*this < that || *this > that);
    }

    friend std::ostream &operator<<(std::ostream &ostream, Reference const &reference)
    {
        return ostream << reference.getName();
    }
};