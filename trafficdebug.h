#ifndef TRAFFICDEBUG_H
#define TRAFFICDEBUG_H

#include <iostream>

enum DebugType {
    DEBUGINFO = 0,
    DEBUGWARNING,
    DEBUGERROR
};

/*!
 * \brief Prints a message.
 */
void printDebug(const std::string &message, int debugType);

#endif // TRAFFICDEBUG_H
