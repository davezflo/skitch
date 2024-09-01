#include "assignment.h"
#include "lvalue.h"
#include "rvalue.h"
#include "exceptions.h"
#include "entity.h"
#include <iostream>

SKAssignment::SKAssignment()
{
    _lValue = NULL;
    _rValue = NULL;
    _op = Operation::Plus;
    _action = NULL;
}

SKAssignment::~SKAssignment()
{

}
SKAssignment *SKAssignment::AllocCopy()
{
    SKAssignment *returnvalue = new SKAssignment();
    returnvalue->SetRValue(_rValue);
    returnvalue->SetLValue(_lValue);
    returnvalue->SetOperation(_op);
    return returnvalue;
}

void SKAssignment::SetLValue(SKLValue *value)
{
    _lValue = value;
}
void SKAssignment::SetRValue(SKRValue *value)
{
    _rValue = value;
}

void SKAssignment::SetOperation(Operation op)
{
    _op = op;
}

void SKAssignment::Operate(SKEntity *entity)
{
    float val = (_op==Operation::Assign)?0:_lValue->GetValue();

    switch(_op)
    {
        case Operation::Assign:
            val = _rValue->GetValue();
            break;
        case Operation::Plus:
            val = val + _rValue->GetValue();
            break;
        case Operation::Minus:
            val = val - _rValue->GetValue();
            break;
        case Operation::Divide:
            val = val / _rValue->GetValue();
            break;
        case Operation::Times:
            val = val * _rValue->GetValue();
            break;
    }
    _lValue->SetValue(val);    
}