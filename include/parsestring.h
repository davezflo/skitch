#ifndef __PARSESTRING__
#define __PARSESTRING__
#include "elementparser.h"

class SKParseString : public SKElementParser
{
    public:
        SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers) override;

};


#endif