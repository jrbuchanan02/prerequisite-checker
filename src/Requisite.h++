#pragma once

#include "Flagged.h++"
#include "Reference.h++"

#include <istream>
#include <string>
#include <vector>

class Requisite : public Flagged {
    Reference course;
protected:
    virtual std::istream &extract ( std::istream &istream ) override;
public:
    Requisite ( ) noexcept = default;
    Requisite ( Requisite const & ) noexcept = default;
    Requisite ( Requisite && ) noexcept = default;
    Requisite ( std::vector < std::string > const &flags , Reference const &course ) noexcept : Flagged ( flags ) , course ( course ) { }
    virtual ~Requisite ( ) = default;

    bool const allowPreviously ( ) const noexcept {
        return hasFlag ( "pre" );
    }
    bool const allowConcurrent ( ) const noexcept {
        return hasFlag ( "con" );
    }

    Reference const &getCourse ( ) const noexcept { return course; }

};

