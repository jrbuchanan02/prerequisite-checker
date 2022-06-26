#pragma once

#include <Flagged.h++>
#include <Reference.h++>
#include <Referred.h++>

#include <istream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Semester;
using SemesterPointer = std::shared_ptr< Semester >;

class Semester : public Flagged, public Referred
{
public:
    Semester ( ) noexcept                  = default;
    Semester ( Semester const & ) noexcept = default;
    Semester ( Semester && ) noexcept      = default;
    Semester ( std::vector< std::string > const &flags,
               Reference const                  &reference ) noexcept :
            Flagged ( flags ),
            Referred ( reference )
    { }
    virtual ~Semester ( ) = default;

    bool const isChecked ( ) const noexcept;

    Semester    &operator= ( Semester const    &) noexcept = default;
    Semester    &operator= ( Semester    &&) noexcept      = default;
    virtual void extract ( ExtractedItem const & ) override;
};
