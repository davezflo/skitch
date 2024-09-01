#include "lvalue.h"
#include "model.h"
#include "context.h"
#include "exceptions.h"
#include "numvar.h"

SKLValue::SKLValue(string varName, SKModel *m)
{
    _model = m;
    _varName = varName;
}

float SKLValue::GetValue()
{
    float returnvalue = 0.0;
    SKVariable *var = _model->GetCurrentContext()->GetVariable(_varName);
    if(var && var->GetType() == NUMBER_TYPE)
        returnvalue = ((SKNumberVariable *)var)->GetValue();
    else
        throw ValueException {_varName + " not a valid lvalue in this context."};
    
    return returnvalue;
}

void SKLValue::SetValue(float value)
{
    SKVariable *var = _model->GetCurrentContext()->GetVariable(_varName);
    if(var && var->GetType() == NUMBER_TYPE)
        ((SKNumberVariable*)var)->SetValue(value);
}