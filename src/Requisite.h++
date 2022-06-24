#pragma once

#include <Flagged.h++>
#include <Reference.h++>

#include <istream>
#include <string>
#include <vector>

class Requisite : public Flagged
{
    Reference course;
    friend class Requisites;
public:
    Requisite() noexcept = default;
    Requisite(Requisite const &) noexcept = default;
    Requisite(Requisite &&) noexcept = default;
    Requisite(std::vector<std::string> const &flags, Reference const &course) noexcept : Flagged(flags), course(course) {}
    virtual ~Requisite() = default;

    bool const allowPreviously() const noexcept;
    bool const allowConcurrent() const noexcept;

    Reference const &getCourse() const noexcept;


    virtual void extract(ExtractedItem const &) override;

    std::uintmax_t counter = 0;
};
