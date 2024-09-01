#ifndef WIN32
#define strtok_s strtok_r
#define strncpy_s strncpy
#endif
#include "regex.h"
#include <iostream>
#include "lex.h"
#include "log.h"

#include <list>
using namespace std;

#define linesize 5000
#define SINGLE_LINE_COMMENT "z"
#define MULTI_LINE_COMMENT_START "zzz"

typedef struct _candidate
{
    SKToken *token;
    int captured;
    int not_captured;
} candidateCapture;

bool SKLexer::Lex(char *text, list<SKToken*> *tokens, bool debug)
{
    bool success = true;
    char *itr = text;
    char* context = NULL;
    char* line = strtok_s(itr, "\n", &context);
    int linecount = 0;
    list<string> lines;
    bool skip = false;
    while(line)
    {
        string s = line;
        bool skipnow = false;
        if(s == MULTI_LINE_COMMENT_START)
        {
             skip = !skip;
             skipnow = true;
        }
        if(!skip && !skipnow && s.substr(0,1)!=SINGLE_LINE_COMMENT)
            lines.push_back(s);
        line = strtok_s(NULL, "\n", &context);
    }

    context = NULL;
    
    char temp[linesize];
    for(auto l: lines)
    {
        linecount++;
        memset(temp, 0, linesize);
        strncpy_s(temp, l.c_str(), l.size());
        char* candidate = strtok_s(temp, " \t", &context);
        list<SKToken*>::iterator it;
        int atposition = 1;
        while(candidate) 
        {
            bool unknownsymbol = true;
            int not_captured = 0;
            list<candidateCapture> _captures;
            for(it = lexers.begin(); it != lexers.end(); ++it)
            {
                if((*it)->Match(candidate))
                {
                    not_captured = strlen(candidate)-((*it)->MatchedValue().length());
                    SKToken *t = (*it)->Copy();
                    
                    candidateCapture c;
                    c.not_captured = not_captured;
                    c.captured = (*it)->MatchedValue().length();
                    c.token = t;
                    _captures.push_back(c);
                    unknownsymbol = false;
                    if(not_captured == 0)
                        break;
                }
            }

            int longest = 0;
            int least = 100;
            SKToken *best = NULL;
            for(auto a: _captures)
            {
                if(a.captured>longest && a.not_captured<=least)
                {
                    best = a.token;
                    longest = a.captured;
                    least = a.not_captured;
                }
            }
            SKLog::Log("captured " + best->GetToken(), SKLogType::Basic);
            not_captured = least;
            tokens->push_back(best);
            
            if(unknownsymbol)
            {
                SKLog::Log(string("Error: unknown symbol: ")+candidate+string(" at (line, token) (")+to_string(linecount)+", "+to_string(atposition) + ")", SKLogType::Basic);
                success = false;
                break;
            }
            if(not_captured == 0)
            {
                candidate = strtok_s(NULL, " \t\n", &context);
                atposition++;
            }   
            else
                candidate += (strlen(candidate)-not_captured);
        }
    }
    return success;
}
