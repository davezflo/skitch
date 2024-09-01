#ifndef __PARSEGENERATE__
#define __PARSEGENERATE__
#include "elementparser.h"

class SKParseGenerate : public SKElementParser
{
    public:
        SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers) override;

};


#endif