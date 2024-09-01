#ifndef __PARSECOLOR__
#define __PARSECOLOR__
#include "elementparser.h"

class SKParseColor : public SKElementParser
{
    public:
        SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers) override;

};


#endif