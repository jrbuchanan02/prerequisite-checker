#pragma once

#include "Reference.h++"
#include "Serial.h++"

#include <istream>

class Referred : public Serial {
    Reference reference;
protected:
    void grabReference ( std::istream &istream ) {
        istream >> reference;
    }
public:
    Referred ( ) noexcept = default;
    Referred ( Referred const & ) noexcept = default;
    Referred ( Referred && ) noexcept = default;
    Referred ( Reference const &reference ) noexcept : reference ( reference ) { }
    virtual ~Referred ( ) = default;

    Reference const &getReference ( ) const noexcept { return reference; }

    Referred &operator = ( Referred const & ) noexcept = default;
    Referred &operator = ( Referred && ) noexcept = default;
};