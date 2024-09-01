#ifndef __PARSEACTION__
#define __PARSEACTION__
#include "elementparser.h"
#include "comptransition.h"
#include "parsenumber.h"

class SKParseAction : public SKElementParser
{
    private:
        SKParseNumber _parseNumber;
    public:
        SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> parsers) override;

    private:
        SKActionThread *ExtractActionThread(SKToken **check, SKContext *context);
       
        bool IsAssign(string token);
        bool IsTransition(string token);
        bool IsCompare(string token);
        bool IsConsume(string token);
       

        Operation ToOperation(string token);
        Comparison ToCompare(string token);
};


#endif