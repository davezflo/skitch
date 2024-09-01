#include "watch.h"
#include "model.h"
#include "context.h"
#include "entity.h"
#include "face.h"
#include "numvar.h"
#include <iostream>
#include <sstream>

void SKWatch::Update(SKModel *model)
{

    _lastSize = 0;
    string display;
    for(auto a: _watches)
    {
        display += a.Update(model);
    }
    _lastSize = display.size();
    cout << "\33[2J" << display << flush;
}

bool SKWatch::WatchLight()
{
    bool returnvalue = false;
    for(auto a: _watches)
    {
        if(a.GetEntity()==WATCH_LIGHT)
            returnvalue = true;
    }
    return returnvalue;
}

bool SKWatch::WatchPoints()
{
    bool returnvalue = false;
    for(auto a: _watches)
    {
        if(a.GetEntity()==WATCH_POINTS)
            returnvalue = true;
    }
    return returnvalue;
}

string SKWatchItem::Update(SKModel *model)
{
    string returnvalue = "";

    SKShape *shape = model->GetShape(_entity);
    if(shape != NULL)
    {
        list<SKFace *> faces;
        shape->GetFaces(faces);
        int at = 0;
        returnvalue += "ENTITY: " + _entity + "\n" + shape->DebugString() + "\n";
        for(auto a: faces)
        {
            if(at == _face || _face == -1)
                returnvalue +=  "("+to_string(at)+")"+a->DebugString(model);
            at++;
        }
    }
    else
    {
        if(_entity == WATCH_CAMERA)
            returnvalue += "CAMERA: \n" + ((SKEntity *)model->GetCamera())->DebugString();
        else if(_entity == WATCH_LIGHT)
            returnvalue += "LIGHT: \n" + ((SKEntity *)model->GetLight())->DebugString();
        else if(_entity == WATCH_FPS)
            returnvalue += "FPS: \t" + model->GetFPS();
        else if(_entity == WATCH_POINTS)
            returnvalue += "POINTS ON SCREEN\n";
        else if(_entity == WATCH_MOUSE)
        {
            SKNumberVariable *_x = (SKNumberVariable *)model->GetGlobalContext()->GetVariable(MOUSEX);
            SKNumberVariable *_y = (SKNumberVariable *)model->GetGlobalContext()->GetVariable(MOUSEY);
            if(_x)
                returnvalue += "MOUSE: \t" + to_string(_x->GetValue()) + ", " + to_string(_y->GetValue()) + "\n";
        }
        else
        {
            SKVariable *v = model->GetGlobalContext()->GetVariable(_entity);
            if(v)
                returnvalue += "VAR: " + v->DebugString();
            else 
            {
                returnvalue += "VAR: + " + _entity + " not found";
            }
        }
    }

    return returnvalue;
}