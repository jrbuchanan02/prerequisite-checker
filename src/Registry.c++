#include "Registry.h++"

#include "Course.h++"
#include "ExtractionMethods.h++"
#include "Keyword.h++"
#include "Plan.h++"
#include "Reference.h++"
#include "Requisites.h++"
#include "Semester.h++"
#include "Serial.h++"

#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

void Registry::clear ( ) noexcept {
    while ( !courses.empty ( ) ) {
        delete courses.back ( );
        courses.pop_back ( );
    } while ( !requisites.empty ( ) ) {
        delete requisites.back ( );
        requisites.pop_back ( );
    } while ( !semesters.empty ( ) ) {
        delete semesters.back ( );
        semesters.pop_back ( );
    } while ( !plans.empty ( ) ) {
        delete plans.back ( );
        plans.pop_back ( );
    }
}

void Registry::copy ( Registry const &registry ) noexcept {
    for ( Course *pcourse : registry.courses ) {
        courses.push_back ( new Course ( *pcourse ) );
    } for ( Requisites *prequisites : registry.requisites ) {
        requisites.push_back ( new Requisites ( *prequisites ) );
    } for ( Semester *psemester : registry.semesters ) {
        semesters.push_back ( new Semester ( *psemester ) );
    } for ( Plan *pplan : registry.plans ) {
        plans.push_back ( new Plan ( *pplan ) );
    }
}

template < typename T >
void slurpFile ( std::vector < T * > &vector , std::ifstream &ifstream ) {
    T *data = new T ( );
    ifstream >> *data;
    vector.push_back ( data );
}


std::istream &Registry::extract ( std::istream &istream ) {
    // we have four keywords that take up one line.
    // courses
    // requisites
    // semesters
    // plans
    // 
    // each take an argument which is the file to slurp with the items. 

    auto maskaroo = [ & ] ( std::string const &keyword , std::string const &path ) {
        std::ifstream file ( path );
        if ( keyword == "courses" ) {
            slurpFile < Course > ( this->courses , file );
        } else if ( keyword == "requisites" ) {
            slurpFile < Requisites > ( this->requisites , file );
        } else if ( keyword == "semesters" ) {
            slurpFile < Semester > ( this->semesters , file );
        } else if ( keyword == "plans" ) {
            slurpFile < Plan > ( this->plans , file );
        }
    };

    auto getPath = [ ] ( std::stringstream &is , std::string const &keyword ) -> std::string {
        std::string line = is.str ( );
        return line.substr ( line.find ( keyword ) + keyword.size ( ) + 1 );
    };

    auto interface = [ & ] ( std::stringstream &is , std::string const keyword ) {
        maskaroo ( keyword , getPath ( is , keyword ) );
    };

    auto coursesFunction = [ & ] ( std::stringstream &is , Serial & ) {
        interface ( is , "courses" );
    };
    auto requisitesFunction = [ & ] ( std::stringstream &is , Serial & ) {
        interface ( is , "requisites" );
    };
    auto semestersFunction = [ & ] ( std::stringstream &is , Serial & ) {
        interface ( is , "semesters" );
    };

    auto plansFunction = [ & ] ( std::stringstream &is , Serial & ) {
        interface ( is , "plans" );
    };

    static Keyword keywords [ ] = {
        Keyword ( "courses" , std::function ( coursesFunction ) ) ,
        Keyword ( "requisites" , std::function ( requisitesFunction ) ) ,
        Keyword ( "semesters" , std::function ( semestersFunction ) ) ,
        Keyword ( "plans" , std::function ( plansFunction ) ) ,
    };

    std::stringstream line;
    std::string temp;
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
        if ( isComment ( temp ) ) continue;
        else for ( auto i = 0LLU; i < sizeof ( keywords ) / sizeof ( keywords [ 0 ] ); i++ ) {
            if ( temp == keywords [ i ].keyword ) {
                keywords [ i ].function ( line , *this );
                break;
            }
        }
    } while ( !istream.eof ( ) );
    return istream;
}

void Registry::runTests ( ) const noexcept {
    for ( Plan *pplan : plans ) {
        std::cout << pplan->getPlanMessage ( *this ) << "\n";
        std::cout << "Press enter to test the next plan.\n";
        std::cin.get ( );
    }
}