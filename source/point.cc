#include "point.h"
#include "model.h"
#include "projection.h"
#include "rvalue.h"
#include <iostream>

 Vector4 SKPoint::Cross(const SKPoint& op)
 {
    Vector4 result;
    result.x = _lastVecNoProjection.y * op._lastVecNoProjection.z- _lastVecNoProjection.z * op._lastVecNoProjection.y;
    result.y = _lastVecNoProjection.z * op._lastVecNoProjection.x - _lastVecNoProjection.x * op._lastVecNoProjection.z;
    result.z = _lastVecNoProjection.x * op._lastVecNoProjection.y - _lastVecNoProjection.y * op._lastVecNoProjection.x;
    return result;
 }

float SKPoint::Dot(const SKPoint& op)
{
    return _lastVecNoProjection.x * op._lastVecNoProjection.x + 
        _lastVecNoProjection.y * op._lastVecNoProjection.y + 
        _lastVecNoProjection.z * op._lastVecNoProjection.z;
}

Vector4 SKPoint::Minus(const SKPoint& op)
{
    Vector4 result;
    result.x = _lastVecNoProjection.x-op._lastVecNoProjection.x;
    result.y = _lastVecNoProjection.y-op._lastVecNoProjection.y;
    result.z = _lastVecNoProjection.z-op._lastVecNoProjection.z;
    result.w = 1;
    return result;
}

void SKPoint::ApplyRotationX(float degrees)
{
    _xMatrix.ApplyRotationX(degrees/180* SKM_PI);
}

void SKPoint::ApplyRotationY(float degrees)
{
    _yMatrix.ApplyRotationY(degrees/180* SKM_PI);
}

void SKPoint::ApplyRotationZ(float degrees)
{
    _zMatrix.ApplyRotationZ(degrees/180* SKM_PI);
}

void SKPoint::ApplyTranslationX(float value)
{
    _translation.rows[3][0] = value;
}

void SKPoint::ApplyTranslationY(float value)
{
    _translation.rows[3][1] = value;
}

void SKPoint::ApplyTranslationZ(float value)
{
    _translation.rows[3][2] = value;
}

void SKPoint::Update(SKProjection* projection)
{
    Vector4 vec (
        _x->GetValue(),
        _y->GetValue(),
        _z->GetValue(),
        1
    );
    
    vec.Apply(_zMatrix);
    vec.Apply(_yMatrix);
    vec.Apply(_xMatrix);
    vec.Apply(_translation);
    _lastVecNoProjection = Vector4(vec);
    _lastVec = projection->Compute(vec);
}

string SKPoint::DebugString()
{
    string returnvalue = " x: " + to_string(_x->GetValue()) + 
    " y: " + to_string(_y->GetValue()) + 
    " z: " + to_string(_z->GetValue());
    return returnvalue;
}

SKPoint *SKPoint::Duplicate(SKModel *model)
{
    SKPoint v(_name, model);
    v.SetX(_x);
    v.SetY(_y);
    v.SetZ(_z);
    v._zMatrix = _zMatrix;
    v._xMatrix = _xMatrix;
    v._yMatrix = _yMatrix;
    v._translation = _translation;
    
    SKPoint *returnvalue = model->AddPoint(&v);
    returnvalue->_template = false;
    return returnvalue;
}