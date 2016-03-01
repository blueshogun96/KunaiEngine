#include "KeDebug.h"
#include <string>
#include <vector>





/*
 * Callstack
 */
std::vector<std::string> KeCallStack;


void KeCallStackPush( std::string function )
{
    KeCallStack.push_back( function );
}

void KeCallStackPop()
{
    KeCallStack.pop_back();
}