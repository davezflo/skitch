#include "parsewatch.h"
#include "watch.h"
#include "model.h"
#include <fstream>
#include <iostream>
#include <sstream>

SKToken *SKParseWatch::Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers)
{
    SKWatch w;
    SKToken *check = token;
    ExtractOnMatch(OpenTok, &check);
    while(PeekNextToken(check)==VarTok ||
          PeekNextToken(check)==GenVarTok)
    {
        string name = ExtractOnMatch(PeekNextToken(check), &check);
        int face = -1;
        int point = -1;

        if(PeekNextToken(check)==FaceTok)
        {
            ExtractOnMatch(FaceTok, &check);
            face = ExtractNumber(&check);
        }

        if(PeekNextToken(check)==PointTok)
        {
            string pointtok = ExtractOnMatch(PointTok, &check);
            stringstream pp(pointtok.substr(1));
            pp >> point;
        }

        w.WatchPoint(name, face, point);
    }
    ExtractOnMatch(CloseTok, &check);
     _model->AddWatch(&w);
    return check->next;
}
