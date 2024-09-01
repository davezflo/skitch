#include "actionthread.h"
#include "model.h"
#include "entity.h"
#include "rvalue.h"
#include <iostream>

SKActionThread::SKActionThread(string name, MemberType memberType, 
                                string memberVariable, vector<SKRValue *> values,
                                string waiton, string ticket)
{
    _actionname = name;
    _membertype = memberType;
    _membervariable = memberVariable;
    _waiton = waiton;
    _ticket = ticket;

    for(auto a : values)
        _values.push_back(a);
}

SKActionThread::~SKActionThread()
{

}

void SKActionThread::Execute(SKModel *model, SKEntity *entity)
{
    SKEntity *object = entity;
    if(_membertype == MemberType::CameraMember)
        object = (SKEntity *)model->GetCamera();
    else if(_membertype == MemberType::LightingMember)
        object = (SKEntity *)model->GetLight();

    model->BuildActionThread(_actionname, 
                            object,
                            _membervariable,
                            _values, _waiton, _ticket);
}

SKActionThread *SKActionThread::AllocCopy()
{
    SKActionThread *returnvalue = new SKActionThread(_actionname, _membertype, _membervariable, _values, _waiton, _ticket);
    return returnvalue;
}