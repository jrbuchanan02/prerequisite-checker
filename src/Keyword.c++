#include "Keyword.h++"

#include "Referred.h++"
#include "Serial.h++"

#include <functional>
#include <sstream>

std::function < void ( std::stringstream & , Serial & ) > wrap ( std::function < void ( std::stringstream & , Referred & ) > function ) {
    return [ & ] ( std::stringstream &stringstream , Serial &serial ) {
        function ( stringstream , ( Referred & ) serial );
    };
}

void referenceFunction ( std::stringstream &stringstream , Referred &referred ) {
    referred.grabReference ( stringstream );
}