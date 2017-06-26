#include "trafficdebug.h"


void printDebug(const std::string &message, int debugType)
{
    switch (debugType) {
    case DEBUGERROR:
        std::cout << "\033[1;31m[ERROR]\033[0m ";
        break;
    case DEBUGWARNING:
        std::cout << "\033[1;33m[WARNING]\033[0m ";
        break;
    case DEBUGINFO:
    default:
        std::cout << "\033[1;32m[GREEN]\033[0m ";
        break;
    }

    std::cout << message << std::endl;
}
