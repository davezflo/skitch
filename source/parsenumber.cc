#include "parsenumber.h"
#include "numvar.h"
#include "context.h"
#include "exceptions.h"


SKToken *SKParseNumber::Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers)
{
    bool negative = false;
    SKToken *check = token;
    string name = ExtractOnMatch(VarTok, &check);

    ExtractOnMatch(AssignTok, &check);

    if(check->next == NULL) throw ParseException{"ParseNumber: Next Token Missing, expecting NegTok or DigitTok."};
    if(check->next->Name() == NegTok) 
    {
        negative = true;
        check = check->next;
    }
    if(check->next == NULL) throw ParseException{"ParseNumber: Next Token Missing, expecting DigitTok."};
    float value = stof(ExtractOnMatch(DigitTok, &check));

    SKNumberVariable var(name);
    var.SetValue(value * (negative?-1:1));
    
    context->AddVariable(&var);
    context->SetScratchString(LASTVAR, name);
   
    return check->next;

}