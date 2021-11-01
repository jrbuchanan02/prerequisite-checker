#include "Requisite.h++"

#include "Flagged.h++"
#include "Reference.h++"

#include <istream>
#include <string>
#include <vector>

std::istream &Requisite::extract ( std::istream &istream ) {
    //std::stringstream line;
    std::string temp;
    // we are expecting pre|con [pre|con] WORD1 WORD2
    istream >> temp;
    addFlag ( temp );
    if ( istream.peek ( ) == 'p' || istream.peek ( ) == 'c' ) {
        istream >> temp;
        addFlag ( temp );
    }
    return istream >> course;
}