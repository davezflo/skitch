#include "parsestring.h"
#include "context.h"
#include "model.h"
#include "strvar.h"

SKToken *SKParseString::Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers)
{
    SKToken *check = token;
    string name = ExtractOnMatch(VarTok, &check);
    ExtractOnMatch(AssignTok, &check);
    string value = ExtractOnMatch(VarTok, &check);

    SKStringVariable var(name);
    var.SetValue(value);
    context->AddVariable(&var);

    context->SetScratchString(LASTVAR, name);

    return check->next;

}