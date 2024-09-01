#include "parseshape.h"
#include "shape.h"
#include "context.h"
#include "model.h"
#include "face.h"
#include "point.h"
#include "exceptions.h"

SKToken *SKParseShape::Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> _parsers)
{
    SKToken *check = token;
    string name = ExtractOnMatch(VarTok, &check);
    ExtractOnMatch(OpenTok, &check);

    //All points must be defined
    //Then all faces
    map<string, SKPoint *> points;
    vector<SKFace *> faces;
    bool done = false;
    while(!done)
        done = !ExtractPoint(&check, points);
    done = false;
    while(!done)
        done = !ExtractFace(&check, points, faces);
    if(check->Name() == CloseTok)
    {
        check = check->next;
        vector<SKPoint *> shapepoints;
        for(auto p: points)
        {
            shapepoints.push_back(p.second);
        }
        SKShape s(shapepoints, faces, name, _model);
        _model->AddShape(&s);
    }

    return check;
}

bool SKParseShape::ExtractPoint(SKToken **check, map<string,SKPoint *>& points)
{
    bool returnvalue = false;
    string name = "";
    try 
    {
        name = ExtractOnMatch(PointTok, check);
    }
    catch(...) 
    {
        return returnvalue;
    } //face is next

    SKPoint p(name, _model);
    
    SKRValue *x, *y, *z;
    x = ExtractRValue(check, _model->GetGlobalContext());
    y = ExtractRValue(check, _model->GetGlobalContext());
    z = ExtractRValue(check, _model->GetGlobalContext());

    p.SetX(x);
    p.SetY(y);
    p.SetZ(z);
    points.insert(pair<string,SKPoint*>(p.GetName(), _model->AddPoint(&p)));

    returnvalue = true;
    return returnvalue;
}

bool SKParseShape::ExtractFace(SKToken **check, map<string, SKPoint *>& points, vector<SKFace *>& faces)
{
    bool returnvalue = false;
    if(PeekNextToken((*check)) == CloseTok)
    {
        *check = (*check)->next;
        return returnvalue;
    }

    ExtractOnMatch(FaceTok, check);
    string pA = ExtractOnMatch(PointTok, check);
    string pB = ExtractOnMatch(PointTok, check);
    string pC = ExtractOnMatch(PointTok, check);
    string border = "_";
    string fill = "_";
    if(PeekNextToken(*check)==VarTok)
        border = ExtractOnMatch(VarTok, check);
    else if(PeekNextToken(*check)==OtherTok)
        ExtractOnMatch(OtherTok, check);

    if(PeekNextToken(*check)==VarTok)
        fill = ExtractOnMatch(VarTok, check);
    else if(PeekNextToken(*check)==OtherTok)
        ExtractOnMatch(OtherTok, check);

    SKPoint *ppA = points[pA];
    SKPoint *ppB = points[pB];
    SKPoint *ppC = points[pC];
    SKColorVariable *pBorder = border=="_"?NULL:(SKColorVariable *)_model->GetGlobalContext()->GetVariable(border);
    SKColorVariable *pFill = fill=="_"?NULL:(SKColorVariable *)_model->GetGlobalContext()->GetVariable(fill);

    if(ppA && ppB && ppC)
    {
        SKFace face(ppA, ppB, ppC, pBorder, pFill);
        SKFace *f = _model->AddFace(&face);
        faces.push_back(f);
        returnvalue = true;
    }
    else
    {
        throw ParseException{"Ill-formed face declaration."};
    }

    return returnvalue;
}