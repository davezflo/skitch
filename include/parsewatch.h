#ifndef __PARSEWATCH__
#define __PARSEWATCH__
#include "elementparser.h"

class SKParseWatch : public SKElementParser
{
    public:
        SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers) override;

};

#endif