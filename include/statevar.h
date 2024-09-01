#include "variable.h"
#include "exceptions.h"
#include <vector>

using namespace std;

#ifndef __STATEVARIABLE__
#define __STATEVARIABLE__

class SKStateVariable : public SKVariable
{
    private:
        string _currentValue;
        vector<string> _possibleValues;

    public:
        SKStateVariable(string name, string current, vector<string> possibles)
        {
            SetName(name);
            _currentValue = current;
            for(auto a: possibles)
                _possibleValues.push_back(a);
            SetType(STATE_TYPE);
        }
        SKVariable *AllocCopy() override
        {
            SKStateVariable *returnvalue = new SKStateVariable(GetName(), _currentValue, _possibleValues);
            return returnvalue;
        }
        void SetValue(string value)
        {
            bool ok = false;
            for(auto a: _possibleValues)
            {
                if(value == a)
                {
                    ok = true;
                    break;
                }
            }
            if(ok)
                _currentValue = value;
            else
                throw StateVarException("Invalid value: " + value + " for variable " + GetName());
        }
        string GetValue() { return _currentValue;}
        string DebugString() override
        {
            return GetType() + "(" + GetName() + "): " + _currentValue + "\n";
        }
  };

#endif
