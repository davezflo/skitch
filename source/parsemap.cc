#include "parsemap.h"
#include "map.h"
#include "infinimap.h"
#include "turtlemap.h"
#include "model.h"
#include "context.h"
#include "exceptions.h"

SKToken *SKParseMap::Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers)
{
    SKToken *check = token;
    string name = ExtractOnMatch(VarTok, &check);
    ExtractOnMatch(OpenTok, &check);
    string maptype;
    string t = ExtractNextToken(&check, false, maptype);
    if(t == VarTok)
    {
        if(maptype == "turtle")
            ExtractTurtleMap(name, &check);
        else if(maptype == "infini")
            ExtractInfiniMap(name, &check);
    }
    
    return check->next;
}

void SKParseMap::ExtractInfiniMap(string name, SKToken **check)
{
    string toss;
    string holdtoken = ExtractNextToken(check, false, toss);
    float value = ExtractNumber(check); 
    SKInfiniMapHold x;
    SKInfiniMapHold y;
    SKInfiniMapHold z;

    if(holdtoken == XTok)
        x.Initialize(true, value);
    else if(holdtoken == YTok)
        y.Initialize(true, value);
    else if(holdtoken == ZTok)
        z.Initialize(true, value);

    SKInfiniMap m(name, x, y, z);
    _model->AddMap((SKMap *)&m);
    string tokname = ExtractNextToken(check, false, toss); //Eat the close
    if(tokname != CloseTok)
        throw ParseException{string("Expected CloseTok, Got ") + tokname};
}

void SKParseMap::ExtractTurtleMap(string name, SKToken **check)
{
    string match;
    string tokname = ExtractNextToken(check, false, match);
    list<SKMapMove> moves;
    while(tokname != CloseTok)
    {
        if(tokname == TurtleForwardTok ||
            tokname == TurtleTurnXTok ||
            tokname == TurtleTurnYTok ||
            tokname == TurtleTurnZTok ||
            tokname == TurtleRotateXTok ||
            tokname == TurtleRotateYTok ||
            tokname == TurtleRotateZTok ||
            tokname == TurtleAdjustXTok ||
            tokname == TurtleAdjustYTok ||
            tokname == TurtleAdjustZTok)
        {
            float amount = ExtractNumber(check);
            SKMapMove::Direction d = SKMapMove::Direction::Forward;
            if(tokname == TurtleTurnZTok) 
                d = SKMapMove::Direction::TurnZ;
            else if(tokname == TurtleTurnYTok) 
                d = SKMapMove::Direction::TurnY;
            else if(tokname == TurtleTurnXTok) 
                d = SKMapMove::Direction::TurnX;
            else if(tokname == TurtleRotateYTok) 
                d = SKMapMove::Direction::RotateY;
            else if(tokname == TurtleRotateXTok) 
                d = SKMapMove::Direction::RotateX;
            else if(tokname == TurtleRotateZTok) 
                d = SKMapMove::Direction::RotateZ;
            else if(tokname == TurtleAdjustXTok)
                d = SKMapMove::Direction::AdjustX;
            else if(tokname == TurtleAdjustYTok)
                d = SKMapMove::Direction::AdjustY;
            else if(tokname == TurtleAdjustZTok)
                d = SKMapMove::Direction::AdjustZ;

            SKMapMove x(d, amount);
            moves.push_back(x);
        }
        else
        {
            SKMapMove x(tokname==TurtleSetPointTok?SKMapMove::Direction::SetPoint
                                                :SKMapMove::Direction::ReturnToPoint, 0);
            moves.push_back(x);
        }
        tokname = ExtractNextToken(check, false, match);
    }
    SKTurtleMap m(name);
    for(auto mm: moves)
        m.AddMove(_model->AddMapMove(&mm));
    _model->AddMap((SKMap *)&m);
}