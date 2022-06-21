#pragma once

class Registry; // fix Registry.h++ including this file

#include "Reference.h++"
#include "Referred.h++"

#include <istream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

class Plan;
using PlanPointer = std::shared_ptr<Plan>;

class Plan : public Referred {
    std::map < Reference , std::vector < Reference > > semesters;
protected:
    virtual std::istream &extract ( std::istream &istream ) override;
public:
    Plan ( ) noexcept = default;
    Plan ( Plan const & ) noexcept = default;
    Plan ( Plan && ) noexcept = default;
    Plan ( Reference const &reference , std::map < Reference , std::vector < Reference > > const &semesters ) noexcept : Referred ( reference ) , semesters ( semesters ) { }
    virtual ~Plan ( ) = default;

    std::string const getPlanMessage ( Registry const & ) const noexcept;

    Plan &operator = ( Plan const & ) noexcept = default;
    Plan &operator = ( Plan && ) noexcept = default;
};

