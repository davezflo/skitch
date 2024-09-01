#include "surface.h"
#include "model.h"
#include "projection.h"
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

SKSurface::SKSurface() {}
SKSurface::SKSurface(const SKSurface& s)
{
    _normalPoint = s._normalPoint;
    for(auto a: s._faces)
        _faces.push_back(a);
    for(auto a: s._renderFaces)
        _renderFaces.push_back(a);
}

void SKSurface::SetNormalPoint(Vector4 np)
{
    _normalPoint = np;
}

string SKSurface::DebugString()
{
    string returnvalue;
    returnvalue = "x: " + to_string(_normalPoint.x) + " y: " + to_string(_normalPoint.y) + 
    " z: " + to_string(_normalPoint.z) + " w: " + to_string(_normalPoint.w);
    return returnvalue;
}

bool SKSurface::operator==(const Vector4& s)
{
    bool returnvalue = false;
    if(_normalPoint == s)
        returnvalue = true;
    return returnvalue;
}

void SKSurface::AddFace(SKFace *f)
{
    _faces.push_back(f);
}

void SKSurface::PrepareForRender(SKModel *model)
{
    for(auto f: _faces)
    {
        if(f->IsFaceVisibleFromThisAngle(model->GetProjection()))
            _renderFaces.push_back(f);
    }
    
    struct 
    {
        bool operator()(SKFace *a, SKFace *b)
        {
            bool returnvalue = a->GetCenter().y<b->GetCenter().y;
            if(abs(a->GetCenter().y - b->GetCenter().y) < 0.1)
                returnvalue = a->Distance()>b->Distance();
            return returnvalue;
        }
    } compareFaceDistance;
    
    std::sort(_renderFaces.begin(), _renderFaces.end(), compareFaceDistance);
}