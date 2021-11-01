#include "Semester.h++"

#include <istream>
#include <sstream>
#include <string>
#include <vector>

std::istream &Semester::extract ( std::istream &istream ) {
    // semesters begin with the "semester" keyword
    std::stringstream line;
    std::string temp;
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
    } while ( temp != "semester" );
    // we can have three values:
    // 1. endsemester (we end the creation of the semester)
    // 2. ref (the reference that the semester uses)
    // 3. [anything else] (a flag!)
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
        if ( temp == "endsemester" ) break;
        else if ( temp == "ref" ) {
            grabReference ( line );
        } else {
            addFlag ( temp );
        }
    } while ( true );
    return istream;
}