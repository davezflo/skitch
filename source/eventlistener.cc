#include "eventlistener.h"

bool IsEvent(string name)
{
    bool returnvalue = false;
    if(name == STR_KEYUP ||
        name == STR_KEYDOWN)
        returnvalue = true;
    return returnvalue;
}