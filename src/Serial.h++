#pragma once

#include <istream>

class Serial {
protected:
    virtual std::istream &extract ( std::istream & ) = 0;
public:

    Serial ( ) noexcept = default;
    Serial ( Serial const & ) noexcept = default;
    Serial ( Serial && ) noexcept = default;
    Serial ( std::istream &istream ) { istream >> *this; }
    virtual ~Serial ( ) = default;

    Serial &operator = ( Serial const & ) noexcept = default;
    Serial &operator = ( Serial && ) noexcept = default;

    friend std::istream &operator >> ( std::istream &istream , Serial &serial ) {
        return serial.extract ( istream );
    }
};