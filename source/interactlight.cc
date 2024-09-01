#include "interactlight.h"
#include "shape.h"
#include "light.h"
#include "face.h"
#include "projection.h"
#include "model.h"
#include "point.h"
#include "maths.h"
#include "surface.h"
#include <map>
#include <list>
#include <vector>
#include <iostream>

using namespace std;

void SKInteractLight::Interact(SKShape *s)
{
    SKLight *light = _model->GetLight();
    list<SKFace *> faces;
    s->GetFaces(faces);
    for(auto a: faces)
        CreateShadow(a, light);
}

vector<SKShape *> SKInteractLight::GetGeneratedShapes()
{
    vector<SKShape *> returnvalue;
    for(auto s: _generatorMap)
    {
        returnvalue.push_back(s.second);
    }
    return returnvalue;
}

void SKInteractLight::CreateShadow(SKFace *f, SKLight *light)
{
    Vector4 lightLocation = light->GetLocation();
    SKPoint *p1, *p2, *p3;
    f->GetPoints(&p1, &p2, &p3, 1);

    Vector4 a, b, c;
    
    Vector4 aa, bb, cc;
    aa = p1->GetRawUnProjectedVector();
    bb = p2->GetRawUnProjectedVector();
    cc = p3->GetRawUnProjectedVector();
    a = aa.Minus(lightLocation);
    b = bb.Minus(lightLocation);
    c = cc.Minus(lightLocation);

    vector<SKSurface> surfaces = _model->GetSurfaces();
    SKSurface winner;
    float ideal = 0;
    bool idealset = false;
    for(auto s: surfaces)
    {
        if(!idealset && s.GetNormalPoint().YNormal())
        {
            winner = s;
            idealset = true;
            ideal = a.y-s.GetNormalPoint().w;
        }
        else if(idealset && s.GetNormalPoint().YNormal() && a.y-s.GetNormalPoint().w < ideal)
        {
            winner = s;
            ideal = a.y-s.GetNormalPoint().w ;
        }
    }
    float floor = 0;// For this to work, we need to determine if the surface has a face that can accept the shadow. myround(winner.GetNormalPoint().w);
    Vector4 a1 = CreateShadowPoint(floor, aa, lightLocation);
    Vector4 b1 = CreateShadowPoint(floor, bb, lightLocation);
    Vector4 c1 = CreateShadowPoint(floor, cc, lightLocation);
   
    vector<Vector4> pp; pp.push_back(a1); pp.push_back(b1); pp.push_back(c1);
    vector<int> pointindicies;
    pointindicies.push_back(0);
    pointindicies.push_back(1); 
    pointindicies.push_back(2);
    list<vector<int>> faces; faces.push_back(pointindicies);

    SKShape *shadowShape = _generatorMap[f];
    if(shadowShape == NULL)
    {
        shadowShape = _model->CreateActiveAnonShape(pp, faces, 0x11, 0x11);
        _generatorMap[f] = shadowShape;
    }
    else
    {
        _model->UpdateActiveAnonShape(_generatorMap[f], pp);
    }
}

Vector4 SKInteractLight::CreateShadowPoint(float surfaceY, Vector4 facepoint, Vector4 light)
{
    float t = (surfaceY-facepoint.y) / (light.y-facepoint.y);
    double newx = (1 - t) * facepoint.x + light.x * t;
    double newy = (1 - t) * facepoint.y + light.y * t;
    double newz = (1 - t) * facepoint.z + light.z * t;

    Vector4 returnvalue(newx, newy, newz, 1);
    return returnvalue;
}