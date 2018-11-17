#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include "version.hh"
#include "tuna-ge.h"

using namespace tunagui;
using namespace tunage;

std::string version(){
    std::stringstream ss{};
    ss << "v" << GUI_MAJOR << "." << GUI_MINOR << "." << GUI_PATCH << "\t\t";
    ss << "Commit: " << Version::GIT_SHA1 << "\tMessage: " << Version::GIT_COMMIT_SUBJECT;
    return ss.str();
}

void doVersion(){
    std::cout << "Tuna GUI" << std::endl;
    std::cout << version() << std::endl;
}

int main(int argc, char** argv) {
    for(int i=0; i<argc; i++){
        if(strcmp(argv[i], "-v") == 0){
            doVersion();
            return 0;
        }
    }
    TunaGE::init();
    TunaGE::enableOriginMarker();
    std::cout << "Library Version: " << TunaGE::version() << std::endl;
    std::cout << "GUI Version: " << version() << std::endl;
    TunaGE::loop();
    return 0;
}