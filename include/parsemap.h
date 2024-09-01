#ifndef __PARSEMAP__
#define __PARSEMAP__
#include "elementparser.h"

class SKParseMap : public SKElementParser
{
    public:
        SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers) override;

    private:
        void ExtractTurtleMap(string name, SKToken **check);
        void ExtractInfiniMap(string name, SKToken **check);
        

};


#endif