#include "Registry.h++"

#include "Course.h++"
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

std::istream &Registry::extract ( std::istream &istream ) {
    // we have four keywords that take up one line.
    // courses
    // requisites
    // semesters
    // plans
    // 
    // each take an argument which is the file to slurp with the items.
    std::stringstream line;
    std::string temp;
    do {
        std::getline ( istream , temp );
        line = std::stringstream ( temp );
        line >> temp;
        std::string path = line.str ( ).substr ( line.str ( ).find ( temp ) + temp.size ( ) + 1 );
        //std::cout << "Will look in path " << path << "\n";
        if ( temp == "courses" ) {
            //std::cout << "Slurping courses file...\n";
            std::ifstream coursesFile ( path );
            //std::cout << "Courses file opened: " << coursesFile.good ( ) << std::endl;
            while ( !coursesFile.eof ( ) ) {
                Course *found = new Course ( );
                coursesFile >> *found;
                courses.push_back ( found );
                //std::cout << "Found Course " << found->getReference ( ) << "\n";
            }
        } else if ( temp == "requisites" ) {
            std::ifstream requisitesFile ( path );
            while ( !requisitesFile.eof ( ) ) {
                Requisites *found = new Requisites ( );
                requisitesFile >> *found;
                requisites.push_back ( found );
            }
        } else if ( temp == "semesters" ) {
            std::ifstream semestersFile ( path );
            while ( !semestersFile.eof ( ) ) {
                Semester *found = new Semester ( );
                semestersFile >> *found;
                semesters.push_back ( found );
            }
        } else if ( temp == "plans" ) {
            std::ifstream plansFile ( path );
            while ( !plansFile.eof ( ) ) {
                Plan *found = new Plan ( );
                plansFile >> *found;
                plans.push_back ( found );
            }
        }
    } while ( !istream.eof ( ) );
    return istream;
}

void Registry::runTests ( ) const noexcept {
    for ( Plan *pplan : plans ) {
        std::cout << pplan->getPlanMessage ( *this ) << "\n";
        std::cout << "Press enter to test the next plan.\n";
        std::cin.get();
    }
}