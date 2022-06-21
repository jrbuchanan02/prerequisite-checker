#pragma once

#include <Serial.h++>

#include <istream>
#include <string>

class Reference : public Serial
{
    std::string contents = "";

protected:
    virtual std::istream &extract(std::istream &istream) override;

public:
    Reference() noexcept = default;
    Reference(Reference const &) noexcept = default;
    Reference(Reference &&) noexcept = default;
    Reference(std::string const &contents) noexcept;

    Reference(std::istream &istream) : Serial(istream) {}
    virtual ~Reference() = default;

    std::string const getName() const noexcept;

    Reference &operator=(Reference const &) noexcept = default;
    Reference &operator=(Reference &&) noexcept = default;

    inline auto operator<=>(Reference const &that) const noexcept
    {
        return contents <=> that.contents;
    }

    inline bool const operator==(Reference const &that) const noexcept
    {
        return contents == that.contents;
    }

    friend inline std::ostream &operator<<(std::ostream &ostream, Reference const &reference)
    {
        return ostream << reference.getName();
    }
};