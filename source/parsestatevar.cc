#include "parsestatevar.h"
#include "context.h"
#include "model.h"
#include "statevar.h"

SKToken *SKParseStateVariable::Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers)
{
    SKToken *check = token;
    string name = ExtractOnMatch(VarTok, &check);
    string defaultvalue  = "";
    vector<string> possibles;
    ExtractOnMatch(AssignTok, &check);
    
    while(PeekNextToken(check) != EndMarkTok)
    {
        string value;
        string token = ExtractNextToken(&check, false, value);
        if(token == OpenTok)
        {
            value = ExtractOnMatch(VarTok, &check);
            possibles.push_back(value);
            defaultvalue = value;
            ExtractOnMatch(CloseTok, &check);
        }
        else
            possibles.push_back(value);
    }
    check = check->next;

    if(defaultvalue == "") 
        throw ParseException("Failed to set default value for state variable: "+name);

    SKStateVariable var(name, defaultvalue, possibles);
    context->AddVariable(&var);

    context->SetScratchString(LASTVAR, name);
    return check->next;

}