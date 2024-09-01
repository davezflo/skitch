#ifndef __PARSENUMBER__
#define __PARSENUMBER__
#include "elementparser.h"

class SKParseNumber : public SKElementParser
{
    public:
        SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers) override;

};


#endif