#include "variable.h"
#include "maths.h"

#ifndef __NUMBERVARIABLE__
#define __NUMBERVARIABLE__

class SKNumberVariable : public SKVariable
{
    protected:
        float _value;

    public:
        SKNumberVariable(string name)
        {
            _value = 0;
            SetName(name);
            SetType(NUMBER_TYPE);
        }
        SKVariable *AllocCopy() override
        {
            SKNumberVariable *returnvalue = new SKNumberVariable(GetName());
            returnvalue->SetValue(_value);
            return returnvalue;
        }
        virtual void SetValue(float value)
        {
            _value = myround(value);
        }
        virtual float GetValue() {return myround(_value);}
        string DebugString() override
        {
            return GetType() + "(" + GetName() + "): " + to_string(_value) + "\n";
        }
};

#endif
