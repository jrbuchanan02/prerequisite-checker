#pragma once

#include <istream>
#include <memory>
#include <vector>

class Requisites;
using RequisitesPointer = std::shared_ptr<Requisites>;

#include <Course.h++>
#include <Plan.h++>
#include <Referred.h++>
#include <Registry.h++>
#include <Requisite.h++>

class Requisites : public Referred
{
    std::vector<Requisite> requisites;

protected:
    virtual std::istream &extract(std::istream &istream) override;

public:
    Requisites() noexcept = default;
    Requisites(Requisites const &) noexcept = default;
    Requisites(Requisites &&) noexcept = default;
    Requisites(std::vector<Requisite> const &requisites) noexcept : requisites(requisites) {}
    virtual ~Requisites() = default;

    bool const meetsRequisite(Course const &) const noexcept;

    std::vector<Requisite> const &getRequisites() const noexcept;

    Requisites &operator=(Requisites const &) noexcept = default;
    Requisites &operator=(Requisites &&) noexcept = default;
};
