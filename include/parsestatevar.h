#ifndef __PARSESTATEVAR__
#define __PARSESTATEVAR__
#include "elementparser.h"

class SKParseStateVariable : public SKElementParser
{
    public:
        SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers) override;

};


#endif