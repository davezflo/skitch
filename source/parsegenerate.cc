#include "parsegenerate.h"
#include "exceptions.h"
#include "generate.h"
#include "model.h"

SKToken *SKParseGenerate::Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers)
{
    SKToken *check = token;
    string name = ExtractOnMatch(VarTok, &check);
    ExtractOnMatch(OpenTok, &check);
    list<string> interactions;

    string shapename = ExtractOnMatch(VarTok, &check);
    string varname = "";
    string tt = ExtractNextToken(&check, true, varname);
    ExtractOnMatch(StartTok, &check);
    float limit = ExtractNumber(&check);
    ExtractOnMatch(EndTok, &check);
    SKGenerator gen;
    gen.Initialize(_model, shapename, tt==VarTok?varname:"", limit);

    if(PeekNextToken(check)==MapAttachTok)
    {
        ExtractOnMatch(MapAttachTok, &check);
        string mapname = ExtractOnMatch(VarTok, &check);
        ExtractOnMatch(StartTok, &check);
        float tilesize = ExtractNumber(&check);
        float distance = ExtractNumber(&check);
        ExtractOnMatch(EndTok, &check);
        gen.SetMap(mapname, tilesize, distance);
    }
    
    while(PeekNextToken(check) == AttachTok)
    {
        ExtractOnMatch(AttachTok, &check);
        string actionname = ExtractOnMatch(VarTok, &check);
        ExtractOnMatch(StartTok, &check);
        vector<float> values;
        while(PeekNextToken(check) != EndTok)
        {
            float x = ExtractNumber(&check);
            values.push_back(x);
        }
        ExtractOnMatch(EndTok, &check);
        gen.AddAction(actionname, values);
    }
    while(PeekNextToken(check) == InteractTok)
    {
        ExtractOnMatch(InteractTok, &check);
        gen.AddInteraction(ExtractOnMatch(VarTok, &check));
    }
    RelativeTo rt = None;
    if(PeekNextToken(check)==OriginTok)
        rt = Origin;
    else if(PeekNextToken(check)==PlayerTok)
        rt = Player;
    else if(PeekNextToken(check)==CameraTok)
        rt = Camera;
    else if(PeekNextToken(check) == MapAttachTok)
        rt = Map;

    if(rt == None) throw ParseException{"Failed to set position for generator."};
    check = check->next;
    float x = ExtractNumber(&check);
    float y = ExtractNumber(&check);
    float z = ExtractNumber(&check);

    gen.SetLocation(rt, x, y, z);
   
    ExtractOnMatch(CloseTok, &check);

    _model->AddGenerator(&gen);

    return check->next;
}
