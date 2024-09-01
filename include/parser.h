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
#ifndef __PARSER__
#define __PARSER__

class SKModel;
class SKFace;
class SKPoint;
class SKVariable;
class SKActionThread;
class SKValue;
class SKContext;
class SKElementParser;
class SKParser
{ 
    private:
        SKModel *_model;
        SKLexer _lexer;
        list<string> _parsedfiles;
        map<string, SKElementParser *> _parsers;

    public:
        SKParser()
        {
            _model = NULL;
            InitializeLexer();
        }
        void Parse(SKModel *model, string filename);
        void AddParser(string keyword, SKElementParser *parser) 
        {
            _parsers.insert(pair<string, SKElementParser *>(keyword, parser));
        }

    private:
        void InitializeLexer();
        bool Lex(string filename, list<SKToken *> *program);
        
        SKToken *ParseUse(SKToken *token);
        
        
};
#endif
