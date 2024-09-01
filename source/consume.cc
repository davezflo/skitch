#include "consume.h"
#include "shape.h"
#include "point.h"
#include "model.h"
#include "context.h"
#include "numvar.h"
#include "projection.h"

#include <iostream>

SKConsume::SKConsume()
{
    _type = ConsumeType::ConsumePoint;
    stickyx = stickyy = stickyz = 0.0;
}

void SKConsume::ConsumePoint(SKRValue *x, SKRValue *y, SKRValue *z)
{
    _type = ConsumeType::ConsumePoint;
    _x = x;
    _y = y;
    _z = z;
}

void SKConsume::ConsumeFace(string border, string fill)
{
    _type = ConsumeType::ConsumeFace;
    _border = border;
    _fill = fill;
}

void SKConsume::ConsumeMousePoint()
{
    _type = ConsumeType::ConsumeMousePoint;
}

void SKConsume::ConsumeUndo()
{
    _type = ConsumeType::ConsumeUndo;
}

SKConsume *SKConsume::AllocCopy()
{
    SKConsume *returnvalue = new SKConsume();
    returnvalue->_type = _type;
    returnvalue->_border = _border;
    returnvalue->_fill = _fill;
    returnvalue->_x = _x;
    returnvalue->_y = _y;
    returnvalue->_z = _z;
    return returnvalue;
}

void SKConsume::Consume(SKEntity *entity)
{
    if(_type == ConsumeType::ConsumePoint)
    {
        float x = _x->GetValue();
        float y = _y->GetValue();
        float z = _z->GetValue();
        if(x == stickyx && y == stickyy && z == stickyz) return;
        stickyx = x; stickyy = y; stickyz = z;
        SKShape *s = dynamic_cast<SKShape *>(entity);
        if(s)
        {
            SKSingleRValue newx(x, entity->GetModel());
            SKSingleRValue newy(-y, entity->GetModel());
            SKSingleRValue newz(z, entity->GetModel());
            SKPoint p("_temp_", entity->GetModel());
            p.SetX((SKRValue *)entity->GetModel()->AddValue((SKValue *)&newx));
            p.SetY((SKRValue *)entity->GetModel()->AddValue((SKValue *)&newy));
            p.SetZ((SKRValue *)entity->GetModel()->AddValue((SKValue *)&newz));
            s->AddPoint(entity->GetModel()->AddPoint(&p));
        }
    }
    else if(_type == ConsumeType::ConsumeFace)
    {
        SKShape *s = dynamic_cast<SKShape *>(entity);
        if(s)
            s->CreateCandidateFace(_border, _fill);
    }
    else if(_type == ConsumeType::ConsumeMousePoint)
    {
        float mousex = ((SKNumberVariable *)entity->GetModel()->GetGlobalContext()->GetVariable(RAW_MOUSEX))->GetValue();
        float mousey = ((SKNumberVariable *)entity->GetModel()->GetGlobalContext()->GetVariable(RAW_MOUSEY))->GetValue();
        Vector4 tt{mousex, mousey, 1, 0};
        SKShape *s = dynamic_cast<SKShape *>(entity);
        if(s)
        {
            s->SelectPointFromXY(tt.x, tt.y);
        }
    }
    else if(_type == ConsumeType::ConsumeUndo)
    {
        SKShape *s = dynamic_cast<SKShape *>(entity);
        if(s)
        {
            s->UndoLastOperation();
        }
    }
}