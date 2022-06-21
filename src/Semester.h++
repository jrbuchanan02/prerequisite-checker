#pragma once

#include "Flagged.h++"
#include "Reference.h++"
#include "Referred.h++"

#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

class Semester;
using SemesterPointer = std::shared_ptr<Semester>;

class Semester : public Flagged , public Referred {
protected:
    virtual std::istream &extract ( std::istream &istream ) override;
public:
    Semester ( ) noexcept = default;
    Semester ( Semester const & ) noexcept = default;
    Semester ( Semester && ) noexcept = default;
    Semester ( std::vector < std::string > const &flags , Reference const &reference ) noexcept : Flagged ( flags ) , Referred ( reference ) { }
    virtual ~Semester ( ) = default;

    bool const isChecked ( ) const noexcept {
        // the only keyword that particularly modifies a semester:
        return !hasFlag ( "unchecked" );
    }

    Semester &operator = ( Semester const & ) noexcept = default;
    Semester &operator = ( Semester && ) noexcept = default;

    friend std::istream &operator >> ( std::istream &istream , Semester &semester ) {
        return semester.extract ( istream );
    }
};

