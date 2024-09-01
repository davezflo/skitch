#ifndef __ELEMENTPARSER__
#define __ELEMENTPARSER__

#include <string>
#include <list>
#include "token.h"
#include "assignment.h"
#include "globalsettings.h"
#include "transition.h"
#include "lex.h"
#include "value.h"
#include <map>
#include <string>
#include <stack>
using namespace std;

class SKToken;
class SKContext;
class SKActionThread;
class SKPoint;
class SKFace;
class SKModel;
class SKParser;
class SKElementParser
{
    protected:
        SKParser *_parser;
        SKModel *_model;
    public:
        SKElementParser() { _parser = NULL; _model = NULL; }
        void Initialize(SKModel *model, SKParser *parser)
        {
            _model = model;
            _parser = parser;
        }
        virtual SKToken *Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers) = 0;
  
    protected:
        string ExtractOnMatch(string, SKToken **check);
        string PeekNextToken(SKToken *token);
        string ExtractNextToken(SKToken **check, bool neggreedy, string& matchvalue);
        SKRValue *ExtractRValue(SKToken **check, SKContext *context);
        SKRValue *ExtractEquation(SKToken **check, SKContext *context);
        float ExtractNumber(SKToken **check);
        bool IsOperableRValueToken(string token);
        bool IsOperationToken(string token);
        bool IsMemberVariable(string v) {return (v.substr(0,2)==OBJECT || v.substr(0,2)==CAMERA || v.substr(0,2)==LIGHTING);}

};

#endif
