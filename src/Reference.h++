#pragma once

#include "Serial.h++"

#include <istream>
#include <string>

class Reference : public Serial {
    std::string name [ 2 ];
protected:
    virtual std::istream &extract ( std::istream &istream ) override {
        return istream >> name [ 0 ] >> name [ 1 ];
    }
public:
    Reference ( ) noexcept = default;
    Reference ( Reference const & ) noexcept = default;
    Reference ( Reference && ) noexcept = default;
    Reference ( std::string const &n1 , std::string const &n2 ) noexcept {
        name [ 0 ] = n1;
        name [ 1 ] = n2;
    }
    Reference ( std::istream &istream ) : Serial ( istream ) { }
    virtual ~Reference ( ) = default;

    std::string const getName ( ) const noexcept { return name [ 0 ] + " " + name [ 1 ]; }

    Reference &operator = ( Reference const & ) noexcept = default;
    Reference &operator = ( Reference && ) noexcept = default;

    auto operator <=> ( Reference const &that ) const noexcept {
        auto one = name[0] <=> that.name[0];
        if (one == std::strong_ordering::equivalent) {
            return name[1] <=> that.name[1];
        } else return one;
    }

    bool const operator == (Reference const &that ) const noexcept {
        return !(*this < that || *this > that);
    }

    friend std::ostream &operator << ( std::ostream &ostream , Reference const &reference ) {
        return ostream << reference.getName ( );
    }
};