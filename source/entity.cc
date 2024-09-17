#include "entity.h"
#include "projection.h"
#include "action.h"
#include "model.h"
#include "variable.h"
#include "log.h"
#include <iostream>
#include <string>
#include "attributevariable.h"
#include "context.h"
#include <sstream>

using namespace std;

SKEntity::~SKEntity()
{
    
}

SKEntity::SKEntity(SKModel *model)
{
    _model = model;
    SKAttributeVariable *v = new SKAttributeVariable(rx, this); _attributes.insert(pair<string, SKAttributeVariable *>(rx, v));
    v = new SKAttributeVariable(ry, this); _attributes.insert(pair<string, SKAttributeVariable *>(ry, v));
    v = new SKAttributeVariable(rz, this); _attributes.insert(pair<string, SKAttributeVariable *>(rz, v));
    v = new SKAttributeVariable(tx, this); _attributes.insert(pair<string, SKAttributeVariable *>(tx, v));
    v = new SKAttributeVariable(ty, this); _attributes.insert(pair<string, SKAttributeVariable *>(ty, v));
    v = new SKAttributeVariable(tz, this); _attributes.insert(pair<string, SKAttributeVariable *>(tz, v));
    v = new SKAttributeVariable(sc, this); _attributes.insert(pair<string, SKAttributeVariable *>(sc, v));
    _needsRender = true;
}
       
void SKEntity::SetAttribute(string member, float value)
{
    _attributes[member]->UpdateValue(value);
    auto it = _registeredCallbacks.find(member);
    if(it != _registeredCallbacks.end())
    {
        for(auto lit = _registeredCallbacks[member].begin();lit!=_registeredCallbacks[member].end();++lit)
        {
            (*lit)(this, member, value);
        }
    }
}

SKAttributeVariable *SKEntity::GetAttributeVariable(string member)
{
    SKAttributeVariable *returnvalue = NULL;
    if(member == pa)
        member = _model->GetCurrentContext()->GetScratchString(member);
    if(_attributes.count(member)!=0)
        returnvalue = _attributes[member];
    return returnvalue;
}

float SKEntity::GetAttribute(string member)
{
    if(member == pa)
        member = _model->GetCurrentContext()->GetScratchString(member);
    return _attributes[member]->GetValue();
}

void SKEntity::RegisterPropertyCallback(string member, void(*callback)(SKEntity *, string, float))
{
    auto it = _registeredCallbacks.find(member);
    if(it != _registeredCallbacks.end())
    {
        list<void(*)(SKEntity *, string, float)> l;
        l.push_back(callback);
        _registeredCallbacks.insert(pair<string, list<void(*)(SKEntity *, string, float)>>(member, l));
    }
    else
    {
        _registeredCallbacks[member].push_back(callback);
    }
}

void SKEntity::SetName(string name)
{
    _name = name;
}

string SKEntity::GetName()
{
    return _name;
}

bool SKEntity::CheckAttribute(string attr)
{
    bool returnvalue = false;
    if(attr == rx || attr == tx || attr == ry ||
       attr == ty || attr == tz || attr == rz ||
       attr == pa) 
        returnvalue = true;
    return returnvalue;
}

string SKEntity::DebugString()
{
    ostringstream db;
    db.precision(4); 
    db << "\ttranslation:   " << GetAttribute(tx) << " " << GetAttribute(ty) << " " << GetAttribute(tz) << endl;
    db << "\trotation   :   " << GetAttribute(rx) << " " << GetAttribute(ry) << " " << GetAttribute(rz) << endl;
    
    for(auto a: _actions)
    {
        db << "\t" << a->DebugString() << endl;
    }

    string returnvalue = db.str();
    return returnvalue; 
}

void SKEntity::Update(SKProjection *projection)
{
    for(auto it : _actions)
        it->Update(this);

    for(auto a : _retiredActions)
    {
        a->Deregister(this);
        _actions.remove(a);
    }   
    _retiredActions.clear();

    for(auto q: _actionQueue)
    {
        string waiton = q.first;
        if(_runningActions.count(waiton)==0)
        {
            if(!_actionQueue[waiton].empty())
            {
                ReadyAction a;
                a = _actionQueue[waiton].front();
                _actionQueue[waiton].pop_front();
                RegisterAndRun(a.action, a.parameters, waiton);
            }
        }
    }
}

void SKEntity::Attach(SKAction *action, vector<float> parameters, string waiton)
{
    //if waiton is not empty, create an action queue for it
    //if waiton has an action queue and the runningActions is full, queue the action
    bool queued = false;
    list<ReadyAction> queue;
    if(_actionQueue.count(waiton)==0)
    {
        if(waiton != "")
            _actionQueue.insert(pair<string, list<ReadyAction>>(waiton, queue)); /*create for future */
    }    
    else
    {
        if(_runningActions.count(waiton))
        {
            ReadyAction ac {action, parameters};
            _actionQueue[waiton].push_back(ac);
            queued = true;
        }
    }
    if(!queued)
        RegisterAndRun(action, parameters, waiton);
}

void SKEntity::RegisterAndRun(SKAction *action, vector<float> parameters, string waiton)
{
    SKLog::Log(string("Running Action ") + action->GetName() + string(" when ") + waiton + string(" occurs."), SKLogType::Verbose);
     action->Register(this, parameters);
    _runningActions.insert(pair<string, SKAction *>(waiton, action));
    _actions.push_back(action);
}

void SKEntity::Detach(SKAction *action)
{
    _retiredActions.push_back(action);

    string waiton = "";
    for(auto it = _runningActions.begin();it!=_runningActions.end();++it)
    {
        if((*it).second == action)
        {
            it = _runningActions.erase(it);
            break;
        }    
    }
}