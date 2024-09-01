#ifndef __PARSESHAPE__
#define __PARSESHAPE__
#include <vector>
#include "elementparser.h"

class SKParseShape : public SKElementParser
{
    public:
        SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers) override;

    private:
        bool ExtractPoint(SKToken **check, map<string, SKPoint *>& points);
        bool ExtractFace(SKToken **check, map<string, SKPoint *>& points, vector<SKFace*>& faces);

};


#endif