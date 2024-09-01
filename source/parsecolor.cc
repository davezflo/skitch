#include "parsecolor.h"
#include "colorvar.h"
#include "context.h"


SKToken *SKParseColor::Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers)
{
    //color name = 0xRR 0xGG 0xBB 0xAA
    SKToken *check = token;
    string name =  ExtractOnMatch(VarTok, &check);
    ExtractOnMatch(AssignTok, &check);
    int red = stoi(ExtractOnMatch(HexTok, &check), NULL, 16);
    int green = stoi(ExtractOnMatch(HexTok, &check), NULL, 16);
    int blue = stoi(ExtractOnMatch(HexTok, &check), NULL, 16);
    int alpha = stoi(ExtractOnMatch(HexTok, &check), NULL, 16);
    
    SKColorVariable var(name);
    var.SetRed(red); var.SetGreen(green);
    var.SetBlue(blue); var.SetAlpha(alpha);

    context->AddVariable(&var);
    context->SetScratchString(LASTVAR, name);

    return check->next;
}