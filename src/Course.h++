#pragma once

class Registry; // fixes registry including Course

#include "Flagged.h++"
#include "Reference.h++"
#include "Referred.h++"
//#include "Registry.h++"
#include "Requisites.h++"

#include <istream>
#include <sstream>
#include <string>
#include <vector>

class Course : public Flagged , public Referred {
    double hours;
    std::vector < Reference > requisites;
    std::string name , desc;
protected:
    virtual std::istream &extract ( std::istream &istream ) override;
public:
    Course ( ) noexcept = default;
    Course ( Course const & ) noexcept = default;
    Course ( Course && ) noexcept = default;
    Course (
        std::vector < std::string > const &flags ,
        Reference const &reference ,
        double const &hours ,
        std::vector < Reference > requisites ,
        std::string const &name ,
        std::string const &desc
    ) noexcept :
        Flagged ( flags ) ,
        Referred ( reference ) ,
        hours ( hours ) ,
        requisites ( requisites ) ,
        name ( name ) ,
        desc ( desc ) { }
    virtual ~Course ( ) = default;

    double const &getHours ( ) const noexcept { return hours; }
    std::string const &getName ( ) const noexcept { return name; }
    std::string const &getDesc ( ) const noexcept { return desc; }

    std::vector < Requisites *> const resolveRequisites ( Registry const & ) noexcept;
    bool const hoursHaveAttribute ( std::string const & ) const noexcept;

    bool const meetsRequisites ( std::vector < std::vector < Reference > > const & , Registry const &);

    Course &operator = ( Course const & ) noexcept = default;
    Course &operator = ( Course && ) noexcept = default;

    friend std::istream &operator >> ( std::istream &istream , Course &course ) {
        return course.extract ( istream );
    }
};

