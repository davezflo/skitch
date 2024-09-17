#include "face.h"
#include "log.h"
#include <math.h>
#include "projection.h"
#include "model.h"
#include "light.h"
#include "exceptions.h"
#include "globalsettings.h"
#include <sstream>

using namespace std;

SKFace::SKFace(SKPoint *pA, SKPoint *pB, SKPoint *pC, 
                SKColorVariable *border, 
                SKColorVariable *fill)
{
    _pointA = pA;
    _pointB = pB;
    _pointC = pC;
    _border = border;
    _fill = fill; 
    if(_border)
        _adjustedBorder.Update(1, *_border);
    if(_fill)
        _adjustedFill.Update(1, *_fill);
    _distance = 1e6;
    _needsUpdate = 5;
    minx = maxx = 0;
}

SKColorVariable *SKFace::GetBorderColor()
{
    if(_border)
        return &_adjustedBorder;
    else
        return NULL;
}

SKColorVariable *SKFace::GetFillColor()
{
    if(_fill)
        return &_adjustedFill;
    else
        return NULL;
}

void SKFace::SetBorderColor(SKColorVariable *cv)
{
    _border = cv;
    _adjustedBorder.Update(1, *cv);
}

void SKFace::SetFillColor(SKColorVariable *cv)
{
    _fill = cv;
    _adjustedFill.Update(1, *cv);
}

string SKFace::DebugString(SKModel *model)
{
    ostringstream db;
    db.precision(4); 
    db << "\tcenter:   " << _center.x << " " << _center.y << " " << _center.z << endl;
    db << "\tnormal:   " << _normal.x << " " << _normal.y << " " << _normal.z << endl;
    db << "\tcolor:    " << _fill->DebugString();
    db << "\tdistance: " << _distance << endl;
    db << "\tA:" << _pointA->DebugString() << " B:" << _pointB->DebugString() << " C:" << _pointC->DebugString() << endl;
    
    string returnvalue = db.str();
    return returnvalue; 
}

void SKFace::GetPoints(SKPoint **pA, SKPoint **pB, SKPoint **pC, float extent)
{
    _pointA->SetExtent(extent); _pointB->SetExtent(extent); _pointC->SetExtent(extent);
    *pA = _pointA;
    *pB = _pointB;
    *pC = _pointC;
}

SKFace *SKFace::AllocCopy()
{
    SKFace *face = new SKFace(_pointA, _pointB, _pointC, _border, _fill);
    return face;
}

Vector4 SKFace::GetCenter()
{
    return _center;
}

void SKFace::ComputeCenter()
{
    Vector4 Aruv = _pointA->GetRawUnProjectedVector();
    Vector4 Bruv = _pointB->GetRawUnProjectedVector();
    Vector4 Cruv = _pointC->GetRawUnProjectedVector();

    float x = Aruv.x + Bruv.x + Cruv.x;
    float y = Aruv.y + Bruv.y + Cruv.y;
    float z = Aruv.z + Bruv.z + Cruv.z;

    Vector4 oldCenter = _center;
    _center.x = x/3; _center.y = y/3; _center.z = z/3; _center.w = 1;
    if(_center != oldCenter)
        _needsUpdate=1;

}

//TODO: Better color capability - ability to change colors
//TODO: Ability to select faces from a shape, copy and move them

void SKFace::ComputeNormal()
{
    Vector4 a, b;
    a = _pointC->Minus(*_pointB);
    b = _pointB->Minus(*_pointA);

    Vector4 oldNormal = _normal;
    _normal = a.Cross(b).Normalize();
    if(_normal != oldNormal)
        _needsUpdate=1;
}

void SKFace::Update(SKModel *m, vector<string> interacts)
{
    SKLight *l = m->GetLight();
    bool canlight = false;
    for(auto a: interacts)
        canlight |= (a == INTERACT_SIMPLELIGHTING || a==INTERACT_ADVANCEDLIGHTING);
    if(l && canlight)
    {
        Vector4 castVector;
        Vector4 lightLocation = l->GetLocation();
        float value = _center.Minus(lightLocation).Normalize().Dot(_normal);
        float multiplier = 0;
        if(value < 0.0)
            multiplier = abs(value);
    
        if(_border)
            _adjustedBorder.Update(multiplier, *_border);
        if(_fill)
            _adjustedFill.Update(multiplier, *_fill);
    }
}

bool SKFace::IsFaceVisibleFromThisAngle(SKProjection *p)
{
    ComputeCenter();
    ComputeNormal();

    float oldDistance = _distance;

    float dot = _center.Minus(p->Eye()).Normalize().Dot(_normal);
    bool ok = signbit(dot);
    ok &= p->PointVisible(_center);
    if(ok)
        _distance = _center.Distance(p->Eye());
    if(myround(_distance) != myround(oldDistance))
        _needsUpdate=1;
    if(!p->IsRendered()) _needsUpdate=1;
    return ok;
}

SKFace *SKFace::Duplicate(SKModel *model, vector<SKPoint *> points)
{
    SKFace f(Locate(_pointA, points),
             Locate(_pointB, points),
             Locate(_pointC, points),
             _border,
             _fill);
    return model->AddFace(&f);
}

SKPoint *SKFace::Locate(SKPoint *p, vector<SKPoint *> points)
{
    for(auto a: points)
    {
        if(p->GetName() == a->GetName())
            return a;
    }
    throw PointException {"point " + p->GetName() + " not found."};
}

void SKFace::ApplyRotationX(float degrees)
{
    _xMatrix.ApplyRotationX(degrees/180* SKM_PI);
}

void SKFace::ApplyRotationY(float degrees)
{
    _yMatrix.ApplyRotationY(degrees/180* SKM_PI);
}

void SKFace::ApplyRotationZ(float degrees)
{
    _zMatrix.ApplyRotationZ(degrees/180* SKM_PI);
}

void SKFace::ApplyTranslationX(float value)
{
    _translation.rows[3][0] = value;
}

void SKFace::ApplyTranslationY(float value)
{
    _translation.rows[3][1] = value;
}

void SKFace::ApplyTranslationZ(float value)
{
    _translation.rows[3][2] = value;
}

void SKFace::ApplyScale(float value)
{
    _xMatrix.ApplyScale(value);
    _yMatrix.ApplyScale(value);
    _zMatrix.ApplyScale(value);
}