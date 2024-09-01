#include "generate.h"
#include "model.h"
#include "projection.h"
#include "shape.h"
#include "map.h"
#include "entity.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

int SKGenerator::_nameIterator = 0;

SKGenerator::SKGenerator()
{
    _location = None;
    _linkedMap = NULL;
    _x = _y = _z = 0;
    _initialized = false;
    _namedIterator = 0;
}

SKGenerator::~SKGenerator()
{
    for(auto it = _cache.begin();it!=_cache.end();++it)
    {
        delete (*it);
    }
}

SKGenerator *SKGenerator::AllocCopy()
{
    SKGenerator *returnvalue = new SKGenerator();
    returnvalue->Initialize(_model, _entity, _varname, _limit);
    returnvalue->SetMap(_map, _tilesize, _distance);
    for(auto a:_actions)
        returnvalue->AddAction(a.action, a.parameters);
    for(auto i: _interactions)
        returnvalue->AddInteraction(i);
    
    returnvalue->SetLocation(_location, _x, _y, _z);
    return returnvalue;
}

void SKGenerator::Initialize(SKModel *model, string entity, string varname, float limit)
{
    _model = model;
    _entity = entity;
    _varname = varname;
    _limit = limit;
}

void SKGenerator::AddAction(string action, vector<float> parameters)
{
    ActionSet ac;
    ac.action = action;
    for(auto it=parameters.begin();it!=parameters.end();++it)
    {
        ac.parameters.push_back((*it));
    }
    _actions.push_back(ac);
}

void SKGenerator::AddInteraction(string interaction)
{
    _interactions.push_back(interaction);
}

void SKGenerator::SetLocation(RelativeTo relative, float x, float y, float z)
{
    _location = relative;
    _x = x; _y = y; _z = z;
}

void SKGenerator::InitializeMap()
{
    if(_linkedMap == NULL && _map != "")
    {
        _linkedMap = _model->GetMap(_map);
        _linkedMap->BuildMap(_tilesize, _x, _y, _z);
    }
}

void SKGenerator::SetMap(string map, float tilesize, float distance)
{
     _map = map;
    _distance = distance;
    _tilesize = tilesize;
}

void SKGenerator::ConstructCache()
{
    if(_cache.size() != 0) return;

    for(int i=0;i<_limit;i++)
    {
        string s;
        if(_varname != "")
            s = _varname + (_limit>1?to_string(_namedIterator):"");
        else 
            s = "__generated__" + to_string(_nameIterator);

        _nameIterator++;
        _namedIterator++;
        SKShape *shape = _model->AddShapeInstance(s, _entity);
        for(auto a: _actions)
            _model->AttachActionToShape(s, a.action, a.parameters);
        for(auto a: _interactions)
            shape->AddInteraction(a);
        _entities.push_back(shape);
        _cache.push_back(new SKSimpleCacheEntry(i));
        _names.push_back(s);
    }
}

void SKGenerator::Update(SKProjection *projection)
{
    InitializeMap();  
    ConstructCache();
    switch(_location)
    {
        case Origin:
            if(!_initialized)
            {
                _entities[0]->SetAttribute(tx, _x);
                _entities[0]->SetAttribute(ty, _y);
                _entities[0]->SetAttribute(tz, _z);
            }
            break;
        case Camera:
            break;
        case Player:
            {
                if(!_initialized)
                {
                    SKShape *ss =  _model->GetShape("player");
                    _entities[0]->SetAttribute(tx, ss->GetAttribute(tx)+_x);
                    _entities[0]->SetAttribute(ty, ss->GetAttribute(ty)+_y);
                    _entities[0]->SetAttribute(tz, ss->GetAttribute(tz)+_z);
                }
            }
            break;
        case Map:
            {
                for(auto c: _entities)
                    c->ResetNeedsRender(false);
                for(auto c: _cache)
                    c->_lastTouched--;

                vector<SKMapLocation> moves = _linkedMap->GetMapLocations(projection, _distance);

                for(auto a: moves)
                {
                    SKEntity *entity = _entities[GetEntry(a.GetHash())];
                    if(entity)
                    {
                        entity->SetAttribute(tx, a._direction.x);
                        entity->SetAttribute(ty, a._direction.y);
                        entity->SetAttribute(tz, a._direction.z); 
                        entity->SetAttribute(rx, a._xRotate);
                        entity->SetAttribute(ry, a._yRotate);
                        entity->SetAttribute(rz, a._zRotate);
                        entity->ResetNeedsRender(true);
                    }
                }
             
            }
            break;
        default:
            break;
    }
    _initialized = true;
}

bool compare(pair<int, int>& a, pair<int, int>& b)
{
    return a.second < b.second;
}

int SKGenerator::GetEntry(string hash)
{
    int returnvalue = -1;
    for(auto a : _cache)
    {
        if(a->_hash == hash)
        {
            returnvalue = a->_entry;
            a->_lastTouched++;
            break;
        }    
    }
    if(returnvalue == -1)
    {
        struct 
        {
            bool operator()(SKSimpleCacheEntry *a, SKSimpleCacheEntry *b)
            {
                return a->_lastTouched < b->_lastTouched;
            }
        } sorter;
        sort(_cache.begin(), _cache.end(), sorter);

        _cache[0]->_hash = hash;
        _cache[0]->_lastTouched = 100;
        returnvalue = _cache[0]->_entry;
    }
    return returnvalue;    
}