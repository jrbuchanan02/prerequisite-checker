
#include <Flagged.h++>
#include <Serial.h++>

#include <istream>
#include <string>
#include <vector>

void Flagged::addFlag ( std::string const &flag )
{
    std::string toAdd = flag;
    while ( toAdd.starts_with ( " " ) ) { toAdd = toAdd.substr ( 1 ); }
    while ( toAdd.ends_with ( " " ) )
    {
        toAdd = toAdd.substr ( 0, toAdd.size ( ) - 1 );
    }
    flags.push_back ( flag );
}

bool const Flagged::hasFlag ( std::string const &flag ) const noexcept
{
    auto position = std::find ( flags.begin ( ), flags.end ( ), flag );
    return position != flags.end ( );
}

Flagged::FlagIterator Flagged::begin ( ) noexcept { return flags.begin ( ); }

Flagged::ConstFlagIterator Flagged::begin ( ) const noexcept
{
    return flags.begin ( );
}

Flagged::FlagIterator Flagged::end ( ) noexcept { return flags.end ( ); }

Flagged::ConstFlagIterator Flagged::end ( ) const noexcept
{
    return flags.end ( );
}

Flagged::ConstFlagIterator Flagged::cbegin ( ) const noexcept
{
    return flags.cbegin ( );
}

Flagged::ConstFlagIterator Flagged::cend ( ) const noexcept
{
    return flags.cend ( );
}

// Flagged::ReverseFlagIterator Flagged::rbegin() noexcept
//{
//     return flags.rbegin();
// }
//
// Flagged::ReverseFlagIterator Flagged::rbegin() const noexcept
//{
//     return flags.rbegin();
// }
//
// Flagged::ReverseFlagIterator Flagged::rend() noexcept
//{
//     return flags.rend();
// }
//
// Flagged::ReverseFlagIterator Flagged::rend() const noexcept
//{
//     return flags.rend();
// }
//
// Flagged::ReverseConstFlagIterator Flagged::crbegin() const noexcept
//{
//     return flags.crbegin();
// }
//
// Flagged::ReverseConstFlagIterator Flagged::crend() const noexcept
//{
//     return flags.crend();
// }