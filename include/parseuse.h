#ifndef __PARSEUSE__
#define __PARSEUSE__
#include "elementparser.h"

class SKParseUse : public SKElementParser
{
    public:
        SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers) override;

};


#endif
