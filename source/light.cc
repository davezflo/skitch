#include "light.h"
#include "model.h"
#include "face.h"
#include "attributevariable.h"
#include "colorvar.h"
#include "context.h"
#include <iostream>
#include "maths.h"

SKLight::SKLight(SKModel *model) : SKEntity(model)
{
    _model = model;
    float x = -1; float y = 10; float z = -5;
    SKEntity::SetAttribute("tx", x);
    SKEntity::SetAttribute("ty", y);
    SKEntity::SetAttribute("tz", z);

    Vector4 zeroVector{0,0,0,1};
    _currentLightPosition = Vector4{x, y, z, 1};
    _fiftyPercentDistance = abs(_currentLightPosition.Distance(zeroVector));
    _changed = true;
    _throw = 5.0;
}

SKLight::~SKLight()
{

}

void SKLight::SetAttribute(string member, float value)
{
    if(member == tx)
        this->SetTx(value);
    else if(member == ty)
        this->SetTy(value);
    else if(member == tz)
        this->SetTz(value);
    else if(member == pa)
        SetAttribute(_model->GetCurrentContext()->GetScratchString(pa), value);
    
    SKEntity::SetAttribute(member, value);
    _changed = true;
}

void SKLight::SetTx(float distance)
{
    _currentLightPosition.x = distance;
}

void SKLight::SetTy(float distance)
{   
    _currentLightPosition.y = distance;
}

void SKLight::SetTz(float distance)
{
    _currentLightPosition.z = distance;
}

Vector4 SKLight::GetProjectedLocation()
{
    return _model->GetProjection()->Compute(_currentLightPosition);
}

Vector4 SKLight::GetLocation()
{
    return _currentLightPosition;
}