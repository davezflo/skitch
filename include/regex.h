#include <string.h>
#include <stdio.h>
#include <string>
#include "state.h"
#ifndef __REGEX__
#define __REGEX__
using namespace std;

class Regex
{   
    string matchValue;
    State *stateChain;
    public:
        Regex();
        Regex(const char *pattern);
        virtual ~Regex();
        string Match(char *value); 
        void SetPattern(const char *pattern);
};

#endif
