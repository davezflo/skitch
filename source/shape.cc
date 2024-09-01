#include "shape.h"
#include "action.h"
#include "projection.h"
#include "interactlight.h"
#include "globalsettings.h"
#include "context.h"
#include "maths.h"
#include "model.h"
#include <algorithm>
#include <iostream>
#include <stack>

struct PointComp
{
    float num;
    SKPoint *p;
};

enum class ShapeOperationType
{
    CreatePoint,
    CreateFace
};

typedef struct 
{
    ShapeOperationType type;
    void *object;
} ShapeOperation;

stack<ShapeOperation> _operations;

void SKShape::AddInteraction(string interaction)
{
    _interacts.push_back(interaction); //TODO: Documentation - can interact with light or with other objects, etc
}

SKShape::SKShape(vector<SKPoint *>points, vector<SKFace *> faces, string name, SKModel *model) : SKEntity(model)
{
    SetName(name);
    for(auto p: points)
        _points.push_back(p);
    for(auto f: faces)
        _faces.push_back(f);
}

void SKShape::AddPoint(SKPoint *point)
{
    _operations.push(ShapeOperation{ShapeOperationType::CreatePoint, point});
    _points.push_back(point);
    point->Live();
    SelectPoint(point);
}

void SKShape::SelectPoint(SKPoint *point)
{
    bool validpoint = false;
    for(auto p: _points)
    {
        if(p == point)
        {
            validpoint = true;
            break;
        }
    }
    if(validpoint)
    {
        for(auto pp: _faceCandidate)
            if(pp==point)
                validpoint = false;
        
        if(validpoint)
            _faceCandidate.insert(_faceCandidate.begin(), point);
        
        if(_faceCandidate.size()>3)
            _faceCandidate.erase(_faceCandidate.end()-1);
    }
}

void SKShape::SelectPointFromXY(float x, float y)
{
    vector<PointComp> pointcomparisons;
    for(auto p: _points)
    {
        p->SetExtent(1);
        float cx = p->GetX();
        float cy = p->GetY(); 
        PointComp pp;
        pp.num = abs(x-cx)+abs(y-cy);
        pp.p = p;
        pointcomparisons.push_back(pp);
    }

    struct 
    {
        bool operator()(PointComp &a, PointComp &b)
        {
            bool returnvalue = a.num<b.num;
            return returnvalue;
        }
    } comparepoints;
    
    std::sort(pointcomparisons.begin(), pointcomparisons.end(), comparepoints);
    //If point within tolerance, then select it
    SelectPoint(pointcomparisons[0].p);
}

void SKShape::CreateCandidateFace(string bordervar, string fillvar)
{
    SKColorVariable *border = (SKColorVariable *)_model->GetGlobalContext()->GetVariable(bordervar);
    SKColorVariable *fill = (SKColorVariable *)_model->GetGlobalContext()->GetVariable(fillvar);
    if(_faceCandidate.size()==3)
    {
        SKFace face(_faceCandidate[2], _faceCandidate[1], _faceCandidate[0], border, fill);
        SKFace *newFace = _model->AddFace(&face);
        _faces.push_back(newFace);
        _faceCandidate.clear();
        _operations.push(ShapeOperation{ShapeOperationType::CreateFace, newFace});
    }   
}

void SKShape::UndoLastOperation()
{
    _faceCandidate.clear();
    if(_operations.size() == 0) return;
    auto x = _operations.top(); _operations.pop();
    if(x.type == ShapeOperationType::CreateFace)
    {
        for(auto it = _faces.begin();it!=_faces.end();++it)
        {
            if(*it == x.object)
            {
                _faces.erase(it);
                break;
            }
        }
        _model->RemoveFace((SKFace *)x.object);
    }
    else if(x.type == ShapeOperationType::CreatePoint)
    {
        //Remove any faces that might have used this point
        vector<SKFace *> remove;
        for(auto it = _faces.begin();it!=_faces.end();++it)
        {
            if((*it)->Uses((SKPoint *)x.object))
            {
                remove.push_back(*it);
                it = _faces.erase(it);
            }
        }
        for(auto f: remove)
            _model->RemoveFace(f);

        //remove the point
        for(auto it = _points.begin();it!=_points.end();++it)
        {
            if(*it == x.object)
            {
                _points.erase(it);
                break;
            }
        }
        _model->RemovePoint((SKPoint *)x.object);
    }
}

Vector4 SKShape::GetCenter()
{
    Vector4 returnvalue(0,0,0,1);
    for(auto it=_faces.begin();it!=_faces.end();++it)
    {
        Vector4 a = (*it)->GetCenter();
        returnvalue.x += a.x;
        returnvalue.y += a.y;
        returnvalue.z += a.z;
    }
    returnvalue.x /= _faces.size();
    returnvalue.y /= _faces.size();
    returnvalue.z /= _faces.size();

    return returnvalue;
}

void SKShape::AdjustShape(Vector4 newCenter)
{
    Vector4 c = GetCenter();
    Vector4 adjust = newCenter.Minus(c);
    SetAttribute(tx, adjust.x);
    SetAttribute(ty, adjust.y);
    SetAttribute(tz, adjust.z);
}

void SKShape::Update(SKProjection *projection)
{
    ProcessLight();
    for(auto f: _faces)
        f->Update(_model, _interacts);
    SKEntity::Update(projection);
}

void SKShape::GetFaces(list<SKFace *>& faces)
{
    for(auto f:_faces)
        faces.push_back(f);
}

SKShape *SKShape::Duplicate(SKModel *model, string name)
{
    SKShape *returnvalue = new SKShape(_points, _faces, name, GetModel());
    returnvalue->DuplicateShape(model);
    return returnvalue;
}

SKShape *SKShape::AllocCopy()
{
    SKShape *returnvalue = new SKShape(_points, _faces, GetName(), GetModel());
    for(auto a: _interacts)
        returnvalue->AddInteraction(a);
    return returnvalue;
}

void SKShape::SetAttribute(string member, float value)
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
    else if(member == pa)
        SetAttribute(_model->GetCurrentContext()->GetScratchString(pa), value);
    
    SKEntity::SetAttribute(member, value);
}

void SKShape::SetRx(float degrees)
{
    for(auto a: _points)
        a->ApplyRotationX(degrees);
    for(auto a: _faces)
        a->ApplyRotationX(degrees);
}

void SKShape::SetRy(float degrees)
{
    for(auto a: _points)
        a->ApplyRotationY(degrees);
    for(auto a: _faces)
        a->ApplyRotationY(degrees);
}

void SKShape::SetRz(float degrees)
{
    for(auto a: _points)
        a->ApplyRotationZ(degrees);
    for(auto a: _faces)
        a->ApplyRotationZ(degrees);
}

void SKShape::SetTx(float distance)
{
    for(auto a: _points)
        a->ApplyTranslationX(distance);
    for(auto a: _faces)
        a->ApplyTranslationX(distance);
}

void SKShape::SetTy(float distance)
{
    for(auto a: _points)
        a->ApplyTranslationY(distance);
    for(auto a: _faces)
        a->ApplyTranslationY(distance);
}

void SKShape::SetTz(float distance)
{
    for(auto a: _points)
        a->ApplyTranslationZ(distance);
    for(auto a: _faces)
        a->ApplyTranslationZ(distance);
}

void SKShape::DuplicateShape(SKModel *model)
{
    vector<SKPoint *> newpoints;
    for(auto a: _points)
        newpoints.push_back(a->Duplicate(model));
    _points.clear();
    for(auto a: newpoints)
        _points.push_back(a);
    
    vector<SKFace *> newfaces;
    //Need to loop through the faces 
    for(auto a: _faces)
        newfaces.push_back(a->Duplicate(model, _points));
    _faces.clear();
    for(auto a: newfaces)
        _faces.push_back(a);
}

bool SKShape::ProcessLight()
{
    bool ok = false;
    for(auto a: _interacts)
        ok |= (a == INTERACT_ADVANCEDLIGHTING);

    if(ok)
        _model->ProcessLightInteraction(this);
    return ok;
}