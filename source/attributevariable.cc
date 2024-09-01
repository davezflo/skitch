#include "attributevariable.h"
#include "maths.h"

SKAttributeVariable::SKAttributeVariable(string name, SKEntity *parent) : SKNumberVariable(name)
{
    _parent = parent;
}

 SKVariable *SKAttributeVariable::AllocCopy()
{
    SKAttributeVariable *returnvalue = new SKAttributeVariable(GetName(), _parent);
    returnvalue->SetValue(_value);
    return returnvalue;
}
void SKAttributeVariable::SetValue(float value)
{
    SKNumberVariable::SetValue(value);
    _parent->SetAttribute(GetName(), _value);
}

float SKAttributeVariable::GetValue()
{
    return _value;
}