#pragma once

#include <Serial.h++>

#include <istream>
#include <string>
#include <vector>

class Flagged : public Serial
{

    std::vector<std::string> flags;

protected:
    void addFlag(std::string const &);
    bool const hasFlag(std::string const &flag) const noexcept;

public:
    using FlagGroup = decltype(flags);
    using FlagIterator = decltype(flags.begin());
    //using ReverseFlagIterator = decltype(flags.rbegin());
    using ConstFlagIterator = decltype(flags.cbegin());
    //using ReverseConstFlagIterator = decltype(flags.crbegin());

    Flagged() noexcept = default;
    Flagged(Flagged const &) noexcept = default;
    Flagged(Flagged &&) noexcept = default;
    Flagged(std::vector<std::string> const &flags) noexcept : flags(flags) {}
    Flagged(std::istream &istream) : Serial(istream) {}
    virtual ~Flagged() = default;

    Flagged &operator=(Flagged const &) noexcept = default;
    Flagged &operator=(Flagged &&) noexcept = default;

    FlagIterator begin() noexcept;
    ConstFlagIterator begin() const noexcept;
    FlagIterator end() noexcept;
    ConstFlagIterator end() const noexcept;

    ConstFlagIterator cbegin() const noexcept;
    ConstFlagIterator cend() const noexcept;

    //ReverseFlagIterator rbegin() noexcept;
    //ReverseFlagIterator rbegin() const noexcept;
    //ReverseFlagIterator rend() noexcept;
    //ReverseFlagIterator rend() const noexcept;

    //ReverseConstFlagIterator crbegin() const noexcept;
    //ReverseConstFlagIterator crend() const noexcept;
};
