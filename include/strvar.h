#include "variable.h"

#ifndef __STRINGVARIABLE__
#define __STRINGVARIABLE__

class SKStringVariable : public SKVariable
{
    private:
        string _value;

    public:
        SKStringVariable(string name)
        {
            SetName(name);
            SetType(STRING_TYPE);
        }
        SKVariable *AllocCopy() override
        {
            SKStringVariable *returnvalue = new SKStringVariable(GetName());
            returnvalue->SetValue(_value);
            return returnvalue;
        }
        void SetValue(string value) {_value = value;}
        string GetValue(string value) { return _value;}
        string DebugString() override
        {
            return GetType() + "(" + GetName() + "): " + _value + "\n";
        }
  };

#endif
