#include "context.h"
#include "variable.h"
#include "numvar.h"
#include "log.h"
#include "model.h"
#include "entity.h"
#include "camera.h"
#include "light.h"
#include <string>
using namespace std;

SKContext::SKContext(string name)
{
    _parentContext = NULL;
    _camera = NULL;
    _lighting = NULL;
    _name = name;
}

SKContext::SKContext(string name, SKModel *model)
{
    _name = name;
    _model = model;
    _camera = model->GetCamera();
    _lighting = model->GetLight();
    _parentContext = model->GetGlobalContext();
}

SKContext::SKContext(SKContext *init, SKContext *global)
{
    _name = init->GetName() + "|";
    _parentContext = global;
    _model = NULL;
    _camera = NULL;
    _lighting = NULL;
    for(auto a: init->_variables)
        AddVariable(a.second);
    for(auto a: init->_scratchStrings)
        _scratchStrings.insert(pair<string, string>(a.first, a.second));
    for(auto a: init->_scratchNumbers)
        _scratchNumbers.insert(pair<string, float>(a.first, a.second));
}

SKContext::~SKContext()
{
    for(auto it = _variables.begin();it!=_variables.end();++it)
    {
        delete((*it).second);
    }
    _variables.clear();
}

void SKContext::Dump()
{
    for(auto a: _variables)
    {
        float value = ((SKNumberVariable *)a.second)->GetValue();
        SKLog::Log(a.first + string("=") + to_string(value), SKLogType::Verbose);
    }
    for(auto a: _scratchStrings)
        SKLog::Log(a.first + string("=") + a.second, SKLogType::Verbose);
    for(auto a: _scratchNumbers)
        SKLog::Log(a.first + string("=") + to_string(a.second), SKLogType::Verbose);
}

vector<string> SKContext::GetVariableNames()
{
    vector<string> returnvalue;
    for(auto v: _variables)
        returnvalue.push_back(v.first);
    return returnvalue;
}

SKVariable *SKContext::AddVariable(SKVariable *variable)
{
    if(_variables.count(variable->GetName())) return _variables[variable->GetName()];
    SKVariable *returnvalue = variable->AllocCopy();
    _variables.insert(pair<string, SKVariable *>(returnvalue->GetName(), returnvalue));
    return returnvalue;
}

SKVariable *SKContext::GetVariable(string name)
{
    SKVariable *returnvalue = NULL;
    if(_variables.count(name))
        returnvalue = _variables[name];
    
    if(returnvalue == NULL)
    {
        if(MemberCheck::FindMemberType(name) == MemberType::ObjectMember && _currentEntity!=NULL)
            returnvalue = (SKVariable *)_currentEntity->GetAttributeVariable(MemberCheck::GetVarMember(name));
        else if(MemberCheck::FindMemberType(name) == MemberType::CameraMember && _camera)
            returnvalue = (SKVariable *)_camera->GetAttributeVariable(MemberCheck::GetVarMember(name));
        else if(MemberCheck::FindMemberType(name) == MemberType::LightingMember && _lighting)
            returnvalue = (SKVariable *)_lighting->GetAttributeVariable(MemberCheck::GetVarMember(name));
        else if(_parentContext)
            returnvalue = _parentContext->GetVariable(name);
    }

    if(returnvalue == NULL)
    {
        SKLog::Log("Warning " + name + string(" not found. CurrentEntity is ") + string(_currentEntity==NULL?"null":"not null") + 
            " Camera is " + string(_camera == NULL?"null":"not null") + string("Parent Context is ") + string(_parentContext==NULL?"null":"not null"), 
            SKLogType::Verbose);
    }

    return returnvalue;
}

void SKContext::SetScratchNumber(string name, float value)
{
    if(_scratchNumbers.count(name)==0)
        _scratchNumbers.insert(pair<string,float>(name, value));
    _scratchNumbers[name] = value;
}

void SKContext::SetScratchString(string name, string value)
{
    if(_scratchStrings.count(name)==0)
        _scratchStrings.insert(pair<string,string>(name, value));
    _scratchStrings[name] = value;
}

string SKContext::GetScratchString(string name)
{
    return _scratchStrings[name];
}

float SKContext::GetScratchNumber(string name)
{
    return _scratchNumbers[name];
}
        
bool SKContext::HasScratchNumber(string name) 
{
    return _scratchNumbers.count(name)==1?true:false;
}

bool SKContext::HasScratchString(string name) 
{
    return _scratchStrings.count(name)==1?true:false;
}
