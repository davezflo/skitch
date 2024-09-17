#include "model.h"
#include "projection.h"
#include "shape.h"
#include "generate.h"
#include "actionthread.h"
#include "camera.h"
#include "log.h"
#include "context.h"
#include "ticket.h"
#include "map.h"
#include "turtlemap.h"
#include "rvalue.h"
#include "watch.h"
#include "consume.h"
#include "light.h"
#include "interactlight.h"
#include <algorithm>

SKModel::SKModel()
{
    _projection = new SKProjection(this);
    _camera = new SKCamera(_projection, this);
    _light = new SKLight(this);
    _globalContext = new SKContext("global context", this);
    _needsRender = true;
    _lightInteractions = new SKInteractLight(this);
    
}

SKModel::~SKModel()
{
    for(auto it = _internalVars.begin();it != _internalVars.end();++it)
    {
        delete (*it);
    }
 
    for(auto it = _watches.begin();it!=_watches.end();++it)
    {
        delete (*it);
    }
    for(auto it = _maps.begin();it!=_maps.end();++it)
    {
        delete (*it).second;
    }
    for(auto it = _mapMoves.begin();it!=_mapMoves.end();++it)
    {
        delete (*it);
    }
    for(auto it = _points.begin();it!=_points.end();++it)
    {
        delete (*it);
    }
    for(auto it = _actions.begin();it!=_actions.end();++it)
    {
        delete (*it).second;
    }
    for(auto it = _assignments.begin();it!=_assignments.end();++it)
    {
        delete (*it);
    }
    for (auto it = _actionthreads.begin(); it != _actionthreads.end(); ++it)
    {
        delete (*it);
    }
    for(auto it = _tickets.begin();it!=_tickets.end();++it)
    {
        delete (*it);
    }
    for(auto it = _consumers.begin();it!=_consumers.end();++it)
    {
        delete (*it);
    }
    for(auto it = _faces.begin();it!=_faces.end();++it)
    {
        delete (*it);
    }
    for(auto it = _shapes.begin();it!=_shapes.end();++it)
    {
        delete (*it);
    }
    for(auto it = _transitions.begin();it!=_transitions.end();++it)
    {
        delete (*it);
    }
    for(auto it = _values.begin();it!=_values.end();++it)
    {
        delete (*it);
    }
    for(auto it = _sequences.begin();it!=_sequences.end();++it)
    {
        delete (*it);
    }
     for(auto it = _generators.begin();it!=_generators.end();++it)
    {
        delete (*it);
    }
    delete(_projection);
    delete(_camera);
    delete(_light);
    delete(_globalContext);
    delete(_lightInteractions);

}

void SKModel::SetRenderDimensions(float x, float y) 
{
    _renderDimensions = Vector4(x, y, 0, 0);
    _projection->InitializeDimensions();
}

Vector4 SKModel::GetRenderDimensions() 
{
    return _renderDimensions;
}

void SKModel::PushContext(SKContext *context)
{
    if(_contextStack.size()>0)
        context->SetParentContext(_contextStack.top());
    _contextStack.push(context);
}

SKContext *SKModel::PopContext()
{
    SKContext *returnvalue = NULL;
    if(_contextStack.size()>0)
    {
        SKContext *c = _contextStack.top();
        if(c)
            _contextStack.pop();
        returnvalue = c;
    }
    return returnvalue;
}

void SKModel::BuildActionThread(string action, 
                            SKEntity *entity, 
                            string parameter,
                            vector<SKRValue *> values,
                            string waiton, string ticket)
{
    //values become static at time of call
    vector<float> parameters;
    for(auto a:values)
    {
        float value = a->GetValue();
        parameters.push_back(value);
    }
    if(_actions.count(action))
    {
        SKLog::Log(string("Threading action ")+action + string(" to entity ") + entity->GetName(), SKLogType::Verbose);
        SKAction *a = _actions[action]->AllocCopy();
        a->SetParameter(parameter);
        a->SetTicket(ticket);
        a->SetParentContext(GetCurrentContext());
        entity->Attach(a, parameters, waiton);
    }
}

SKShape *SKModel::AddShapeInstance(string name, string prototype)
{
    SKShape *returnvalue = NULL;
    for(auto it=_shapes.begin();it!=_shapes.end();++it)
    {
        if((*it)->GetName() == prototype)
        {
            returnvalue = (*it)->Duplicate(this, name);
            _liveShapes.insert(std::pair<string, SKShape *>(name, returnvalue));
        }
    }
    return returnvalue;
}

SKShape *SKModel::GetShapeTemplate(string name)
{
    SKShape *returnvalue = NULL;
    for(auto it=_shapes.begin();it!=_shapes.end();++it)
    {
        if((*it)->GetName() == name)
        {
            returnvalue = (*it);
            break;
        }
    }
    return returnvalue;
}

vector<SKShape *> SKModel::GetShapes()
{
    vector<SKShape *> returnvalue;
    for(auto a: _liveShapes)
    {
        returnvalue.push_back(a.second);
    }
    return returnvalue;
}

SKShape *SKModel::GetShape(string name)
{
    if(_liveShapes.count(name) != 0)
        return _liveShapes[name];
    else 
        return NULL;
}

SKMap *SKModel::GetMap(string name)
{
    return _maps[name];
}

SKWatch *SKModel::AddWatch(SKWatch *watch)
{
    SKWatch *returnvalue = watch->AllocCopy();
    _watches.push_back(returnvalue);
    return returnvalue;
}

bool SKModel::WatchPoints()
{
    bool returnvalue = false;
    for(auto a: _watches)
    {
        returnvalue |= a->WatchPoints();
    }
    return returnvalue;
}

SKPoint *SKModel::AddPoint(SKPoint *point)
{
    SKPoint *returnvalue = point->AllocCopy();
    _points.push_back(returnvalue);
    return returnvalue;
}

void SKModel::RemovePoint(SKPoint *point)
{
    for(auto it=_points.begin();it!=_points.end();++it)
    {
        if(*it == point)
        {
            _points.erase(it);
            delete(point);
            break;
        }
    }
}

SKFace *SKModel::AddFace(SKFace *face)
{
    SKFace *returnvalue = face->AllocCopy();
    _faces.push_back(returnvalue);
    return returnvalue;
}

void SKModel::RemoveFace(SKFace *face)
{
    for(auto it=_faces.begin();it!=_faces.end();++it)
    {
        if(*it == face)
        {
            _faces.erase(it);
            delete(face);
            break;
        }
    }
}

SKShape *SKModel::AddShape(SKShape *shape)
{
    SKShape *returnvalue = shape->AllocCopy();
    _shapes.push_back(returnvalue);
    return returnvalue;
}

SKConsume *SKModel::AddConsumer(SKConsume *consume)
{
    SKConsume *returnvalue = consume->AllocCopy();
    _consumers.push_back(returnvalue);
    return returnvalue;
}

SKMap *SKModel::AddMap(SKMap *map)
{
    SKMap *returnvalue = map->AllocCopy();
    _maps.insert(pair<string, SKMap *>(returnvalue->GetName(), returnvalue));
    return returnvalue;
}

SKMapMove *SKModel::AddMapMove(SKMapMove *move)
{
    SKMapMove *returnvalue = move->AllocCopy();
    _mapMoves.push_back(returnvalue);
    return returnvalue;
}

SKAssignment *SKModel::AddAssignment(SKAssignment *assignment)
{
    SKAssignment *returnvalue = assignment->AllocCopy();
    _assignments.push_back(returnvalue);
    return returnvalue;
}

SKTransition *SKModel::AddTransition(SKTransition *transition)
{
    SKTransition *returnvalue = transition->AllocCopy();
    _transitions.push_back(returnvalue);
    return returnvalue;
}

SKPunchTicket *SKModel::AddTicketPunch(SKPunchTicket *ticketpunch)
{
    SKPunchTicket *returnvalue = ticketpunch->AllocCopy();
    _tickets.push_back(returnvalue);
    return returnvalue;
}

SKActionThread *SKModel::AddActionThread(SKActionThread *thread)
{
    SKActionThread *returnvalue = thread->AllocCopy();
    _actionthreads.push_back(returnvalue);
    return returnvalue;
}

SKValue *SKModel::AddValue(SKValue *value)
{
    SKValue *returnvalue = value->AllocCopy();
    _values.push_back(returnvalue);
    return returnvalue;
}

SKAction *SKModel::AddAction(SKAction *action)
{
    SKAction *returnvalue = action->AllocCopy();
    _actions.insert(pair<string, SKAction *>(returnvalue->GetName(), returnvalue));
    return returnvalue;
}

void SKModel::DeleteAction(SKAction* action)
{
    if(_actions.count(action->GetName())==0)
        delete(action);
}

SKGenerator *SKModel::AddGenerator(SKGenerator *generator)
{
    SKGenerator *returnvalue = generator->AllocCopy();
    _generators.push_back(returnvalue);
    return returnvalue;
}

SKActionSequence *SKModel::AddActionSequence(SKActionSequence *seq)
{
    SKActionSequence *returnvalue = seq->AllocCopy();
    _sequences.push_back(returnvalue);
    return returnvalue;
}

void SKModel::AttachActionToShape(string shape, string action, vector<float> parameters)
{
    if(_actions.count(action))
        _liveShapes[shape]->Attach(_actions[action], parameters, "");
}

void SKModel::SortAndPrepareSurfaces()
{
    struct 
    {
        bool operator()(SKSurface &a, SKSurface &b)
        {
            bool returnvalue = a.GetNormalPoint().y<b.GetNormalPoint().y;
            return returnvalue;
        }
    } compareSurfaceY;
    
    std::sort(_surfaces.begin(), _surfaces.end(), compareSurfaceY);
    vector<SKSurface>::iterator it;
    for(it = _surfaces.begin();it!=_surfaces.end();++it)
        (*it).PrepareForRender(this);
}

void SKModel::AddFaceToSurface(SKFace *face, Vector4 nd)
{
    bool found = false;
    int count = _surfaces.size();
    for(int i=0;i<count;i++)
    {
        found = (_surfaces[i] == nd);
        if(found) 
        {
            _surfaces[i].AddFace(face);
            break;
        }
    }
    if(!found)
    {
        SKSurface s;
        s.SetNormalPoint(nd);
        s.AddFace(face);
        _surfaces.push_back(s);
    }
}


void SKModel::CalculateSurfaces()
{
    _surfaces.clear();
    _allfaces.clear();

    vector<SKShape *> shapes = GetShapes();
    list<SKShape *>::iterator it;
    for(auto a: shapes)
    {
        if(!a->NeedsRender()) continue;
        list<SKFace *> faces;
        a->GetFaces(faces);
        for(auto f: faces)
        {
            if(f->IsFaceVisibleFromThisAngle(GetProjection()))
                _allfaces.push_back(f);
        }
    }
    //Get the shadows
    shapes = _lightInteractions->GetGeneratedShapes();
    for(auto a: shapes)
    {
        if(!a->NeedsRender()) continue;
        list<SKFace *> faces;
        a->GetFaces(faces);
        for(auto f: faces)
        {
            if(f->IsFaceVisibleFromThisAngle(GetProjection()))
                _allfaces.push_back(f);
        }
    }

    for(auto f: _allfaces)
    {
        Vector4 n = f->GetNormal();
        Vector4 center = f->GetCenter();
       
        if(n.YNormal())
            n.w = center.y;
        else if(n.XNormal())
            n.w = center.x;
        else if(n.ZNormal())
            n.w = center.z;
            
        AddFaceToSurface(f, n);
    }
}

void SKModel::Update()
{
    _projection->Update();

    for(auto a: _generators)
        a->Update(_projection);

    GetCamera()->Update(_projection);

    for(auto a: _liveShapes)
        a.second->Update(_projection);

    for(auto a: _points)
    {
        if(!a->IsTemplate()) 
            a->Update(_projection);
    }
        
    CalculateSurfaces();
    SortAndPrepareSurfaces();

    for(auto a: _watches)
        a->Update(this);

    LogTime();
}

void SKModel::LogTime()
{
    auto newTime = chrono::high_resolution_clock::now();
    auto ms = chrono::duration_cast<std::chrono::milliseconds>(newTime-_lastTime);
    _lastTime = newTime;
    int totalms = (int)ms.count();
    float fps = 1000/(float)totalms;
    _fps = to_string(int(fps));
}

void SKModel::RegisterKeyListener(string key, KeyState state, EventListener *l)
{
    string k = key + to_string((int)state);
    list<EventListener *> ll;
    if(!_listeners.count(k))
        _listeners.insert(pair<string,list<EventListener *>>(k, ll));
    _listeners[k].push_back(l);
}

void SKModel::RegisterMouseListener(MouseState state, EventListener *l)
{
    string k = to_string((int)state);
    list<EventListener *> ll;
    if(!_listeners.count(k))
        _listeners.insert(pair<string, list<EventListener *>>(k, ll));
    _listeners[k].push_back(l);
}

void SKModel::FireMouseEvent(float x, float y, MouseState state)
{
    string k = to_string((int)state);
    if(_listeners.count(k))
    {
        list<EventListener *> ll = _listeners[k];
         for(auto it=ll.begin();it!=ll.end();++it)
         {
            (*it)->MouseEvent(x, y, state);
         }
    }
}

//TODO: When attaching an action to two different entities, key firing will only occur for one of these. 
//      Expected behavior is that each will fire something on the key event (even if same action, just multi-mapped)
void SKModel::FireKeyEvent(string key, KeyState state)
{
     string k = key + to_string((int)state);
     if(_listeners.count(k))
     {
         list<EventListener *> ll = _listeners[k];
         for(auto it=ll.begin();it!=ll.end();++it)
         {
            (*it)->KeyEvent(key, state);
         }
     }
}

void SKModel::UpdateActiveAnonShape(SKShape *s, vector<Vector4> pointValues)
{
    vector<SKPoint *> points = s->GetPoints();
    if(pointValues.size() != points.size()) return;
    for(unsigned int i=0;i<points.size();i++)
    {
        ((SKSingleRValue *)points[i]->GetXRValue())->ReplaceValue(pointValues[i].x);
        ((SKSingleRValue *)points[i]->GetYRValue())->ReplaceValue(pointValues[i].y);
        ((SKSingleRValue *)points[i]->GetZRValue())->ReplaceValue(pointValues[i].z);
    }
}

SKShape *SKModel::CreateActiveAnonShape(vector<Vector4> points, list<vector<int>> faces, uint32_t fill, uint32_t border)
{
    SKColorVariable fillcolor(fill);
    SKColorVariable bordercolor(border);
    vector<SKPoint *> newpoints;
    for(const auto& p: points)
    {
        SKPoint pp("", this);
        SKSingleRValue x(to_string(p.x), this, false), y(to_string(p.y), this, false), z(to_string(p.z), this, false);
        pp.SetX((SKRValue *)AddValue(&x)); pp.SetY((SKRValue *)AddValue(&y)); pp.SetZ((SKRValue *)AddValue(&z));
        SKPoint *fullpoint = AddPoint(&pp);
        fullpoint->Live();
        newpoints.push_back(fullpoint);
    }
    SKColorVariable *fc = (SKColorVariable *)AddAnonVariable(&fillcolor);
    SKColorVariable *fb = (SKColorVariable *)AddAnonVariable(&bordercolor);
    vector<SKFace *> newfaces;
    for(auto p: faces)
    {
        SKFace f(newpoints[p[0]], newpoints[p[1]], newpoints[p[2]], fb, fc);
        newfaces.push_back(AddFace(&f));
    }
    SKShape s(newpoints, newfaces, "", this);
    return AddShape(&s);
}

SKVariable *SKModel::AddAnonVariable(SKVariable *v)
{
    SKVariable *returnvalue = v->AllocCopy();
    _internalVars.push_back(returnvalue);
    return returnvalue;
}

void SKModel::ProcessLightInteraction(SKShape *s)
{
    _lightInteractions->Interact(s);
}