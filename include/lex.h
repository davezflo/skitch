#include "token.h"
#include "tokensym.h"
#include <list>
using namespace std;
#ifndef __SKLEX__
#define __SKLEX__

class SKLexer
{
private:
    list<SKToken*> lexers;
    bool _initialized = false;
public:
    SKLexer() {}

    virtual ~SKLexer()
    {
        list<SKToken *>::iterator it;
        for(it = lexers.begin();it != lexers.end();++it)
        {
            SKToken *t = (*it);
            delete(t);
        }
    }
    bool Initialized() {return _initialized;}
    void AddToLexer(string name, const char *match)
    {
        _initialized = true;
        lexers.push_back(new SKToken(name, match));
    }

    bool Lex(char *text, list<SKToken *> *tokens, bool debug);
};

#endif
