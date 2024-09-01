#include "camera.h"
#include "globalsettings.h"
#include "maths.h"
#include "log.h"
#include "model.h"
#include "context.h"
#include "attributevariable.h"
#include <sstream>

SKCamera::SKCamera(SKProjection *projection, SKModel *model) : SKEntity(model)
{
    _projection = projection;
    _model = model;
    SKEntity::SetAttribute("tx", _projection->Eye().x);
    SKEntity::SetAttribute("ty", _projection->Eye().y);
    SKEntity::SetAttribute("tz", _projection->Eye().z);

    SKAttributeVariable *v = new SKAttributeVariable(lx, this); 
    _attributes.insert(pair<string, SKAttributeVariable *>(lx, v));
    v = new SKAttributeVariable(ly, this); _attributes.insert(pair<string, SKAttributeVariable *>(ly, v));
    v = new SKAttributeVariable(lz, this); _attributes.insert(pair<string, SKAttributeVariable *>(lz, v));
}

SKCamera::~SKCamera()
{

}

void SKCamera::SetAttribute(string member, float value)
{
    if(member == rx)
        this->SetRx(value);
    else if(member == ry)
        this->SetRy(value);
    else if(member == rz)
        this->SetRz(value);
    else if(member == tx)
        this->SetTx(value);
    else if(member == ty)
        this->SetTy(value);
    else if(member == tz)
        this->SetTz(value);
    else if(member == lx)
        this->SetLx(value);
    else if(member == ly)
        this->SetLy(value);
    else if(member == lz)
        this->SetLz(value);
    else if(member == pa)
        SetAttribute(_model->GetCurrentContext()->GetScratchString(pa), value);
    
    SKEntity::SetAttribute(member, value);
}

void SKCamera::SetRx(float degrees)
{
    _projection->RotateX(degrees);
}

void SKCamera::SetRy(float degrees)
{
    _projection->RotateY(degrees);
}

void SKCamera::SetRz(float degrees)
{
    _projection->RotateZ(degrees);
}

void SKCamera::SetLx(float distance)
{
    SKLog::Log(string("Camera Lx: ")+to_string(distance), SKLogType::Basic);
    _projection->LookAtX(distance);
}

void SKCamera::SetLy(float distance)
{
    SKLog::Log(string("Camera Ly: ")+to_string(distance), SKLogType::Basic);
    _projection->LookAtY(distance);
}

void SKCamera::SetLz(float distance)
{
    SKLog::Log(string("Camera Lz: ")+to_string(distance), SKLogType::Basic);
    _projection->LookAtZ(distance);
}

void SKCamera::SetTx(float distance)
{
    SKLog::Log(string("Camera Tx: ")+to_string(distance), SKLogType::Basic);
    _projection->TranslateX(distance);
}

void SKCamera::SetTy(float distance)
{
    SKLog::Log(string("Camera Ty: ")+to_string(distance), SKLogType::Basic);
    _projection->TranslateY(distance);
}

void SKCamera::SetTz(float distance)
{
    SKLog::Log(string("Camera Tz: ")+to_string(distance), SKLogType::Basic);
    _projection->TranslateZ(distance);
}

string SKCamera::DebugString()
{
    ostringstream db;
    db.precision(4); 
    db << "\ttranslation:   " << GetAttribute(tx) << " " << GetAttribute(ty) << " " << GetAttribute(tz) << endl;
    db << "\trotation   :   " << GetAttribute(rx) << " " << GetAttribute(ry) << " " << GetAttribute(rz) << endl;
    db << "\tlook at    :   " << GetAttribute(lx) << " " << GetAttribute(ly) << " " << GetAttribute(lz) << endl;
    
    string returnvalue = db.str();
    return returnvalue; 
}