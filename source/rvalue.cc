#include "rvalue.h"
#include "model.h"
#include "context.h"
#include "numvar.h"
#include "variable.h"
#include "exceptions.h"
#include "math.h"
#include <iostream>

SKSingleRValue::SKSingleRValue(string varName, SKModel *m, bool applynegative) : SKRValue(m)
{
    _model = m;
    _isDigit = false;
    _applyNegative =  applynegative;
    _varName = varName;
}

SKSingleRValue::SKSingleRValue(float value, SKModel *m) : SKRValue(m)
{
    _model = m;
    _isDigit = true;
    _rawValue = value;
    _applyNegative = false;
    _varName = "";
}

void SKSingleRValue::ReplaceValue(float newValue)
{
    if(!_isDigit) return;
    _rawValue = newValue;
}

float SKSingleRValue::GetValue()
{
    if(_isDigit) return _rawValue * (_applyNegative?-1:1);
    float returnvalue = 0.0;
    SKVariable *var = _model->GetCurrentContext()->GetVariable(_varName);
    if(var && var->GetType() == NUMBER_TYPE)
        returnvalue = ((SKNumberVariable *)var)->GetValue();
    else
    {
        if(IsDigit(_varName))
            returnvalue = _rawValue;
        else
            throw ValueException {_varName + " not a valid rvalue in this context."};
    }

    returnvalue = returnvalue * (_applyNegative?-1:1);

    return returnvalue;
}

bool SKSingleRValue::IsDigit(string var)
{
    bool ok = true;
    for(auto it=var.begin();it!=var.end();++it)
    {
        if(!isdigit(*it) && ((*it)!='-' &&(*it)!='.'))
            ok = false;
    }
    if(ok)
    {
        _rawValue = stof(var.c_str());
        _isDigit = true;
    }
    return ok;
}


 SKOperationRValue::SKOperationRValue(SKModel *m, SKStackOperation op) : SKRValue(m)
{
    _operation = op;
    _cachedValue = 0.0;
}

SKValue *SKOperationRValue::AllocCopy() 
{
    SKOperationRValue *returnvalue = NULL;
    returnvalue = new SKOperationRValue(_model, _operation);
    return returnvalue;
}

float SKOperationRValue::GetValue() 
{
    return _cachedValue;
}

bool SKOperationRValue::BinaryOperation(SKStackOperation op)
{
    bool returnvalue = false;
    if(op == SKStackOperation::StackPlus||
        op == SKStackOperation::StackMinus ||
        op == SKStackOperation::StackDivide ||
        op == SKStackOperation::StackMultiply||
        op == SKStackOperation::StackModulus)
        returnvalue = true;
    return returnvalue;
}

void SKOperationRValue::GetValue(stack<float>& stack) 
{
    float returnvalue = 0.0;
    float a = 0.0;
    float b = 0.0;
    if(BinaryOperation(_operation))
    {
        a = stack.top(); stack.pop();
        b = stack.top(); stack.pop();    
    }
    else
    {
        a = stack.top(); stack.pop();
    }
    
    switch(_operation)
    {
        case SKStackOperation::StackPlus:
            returnvalue = (a + b);
            break;
        case SKStackOperation::StackMinus:
            returnvalue = (b - a);
            break;
        case SKStackOperation::StackMultiply:
            returnvalue = (b * a);
            break;
        case SKStackOperation::StackDivide:
            returnvalue = (b / a);
            break;
        case SKStackOperation::StackModulus:
            returnvalue = int((int(b) % int(a)));
            if(returnvalue < 0) returnvalue += a;
            break;
        case SKStackOperation::StackSin:
            returnvalue = sin(a);
            break;
        case SKStackOperation::StackCos:
            returnvalue = cos(a);
            break;
        case SKStackOperation::StackSign:
            returnvalue = signbit(a)?-1:1;
            break;
        case SKStackOperation::StackAbs:
            returnvalue = abs(a);
            break;
        default:
            break;
    }
    stack.push(returnvalue);
    _cachedValue = returnvalue;
}

SKEquationRValue::SKEquationRValue(SKModel *m, list<SKRValue *> values) : SKRValue(m)
{
    _values = values;
}

SKRValue *SKEquationRValue::AllocCopy()
{
    SKEquationRValue *returnvalue = new SKEquationRValue(_model, _values);
    return returnvalue;
}

float SKEquationRValue::GetValue()
{
    stack<float> stack;
    for(auto a: _values)
        a->GetValue(stack);
    
    return stack.top();
}