#include "parseuse.h"
#include "context.h"
#include "parser.h"

SKToken *SKParseUse::Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers)
{
    SKToken *check = token;
    string filename = ExtractOnMatch(VarTok, &check);
    
    filename += ".sk";
    _parser->Parse(_model, filename);

    return check->next;
}