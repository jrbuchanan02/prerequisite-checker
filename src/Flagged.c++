#include "Flagged.h++"

#include "Serial.h++"

#include <istream>
#include <string>
#include <vector>

bool const Flagged::hasFlag ( std::string const &flag ) const noexcept {
    for ( std::string contained : flags ) {
        if ( contained == flag ) return true;
    } return false;
}