#pragma once

#include "Serial.h++"

#include <istream>
#include <string>
#include <vector>

class Flagged : public Serial {
    std::vector < std::string > flags;
protected:
    void addFlag ( std::string const &flag ) { flags.push_back ( flag ); }
    bool const hasFlag ( std::string const &flag ) const noexcept;
public:
    Flagged ( ) noexcept = default;
    Flagged ( Flagged const & ) noexcept = default;
    Flagged ( Flagged && ) noexcept = default;
    Flagged ( std::vector < std::string > const &flags ) noexcept : flags ( flags ) { }
    Flagged ( std::istream &istream ) : Serial ( istream ) { }
    virtual ~Flagged ( ) = default;

    Flagged &operator = ( Flagged const & ) noexcept = default;
    Flagged &operator = ( Flagged && ) noexcept = default;
};

