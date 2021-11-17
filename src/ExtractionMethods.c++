#include "ExtractionMethods.h++"

#include <istream>
#include <sstream>
#include <string>


bool const extractToKeyword ( std::istream &istream , std::string const &keyword ) {
    std::stringstream line;
    std::string temp;
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
    } while (temp != keyword && !istream.eof ( ) );
    return !istream.eof ( );
}

bool const isComment ( std::string const &word ) {
    return word.front ( ) == '#';
}