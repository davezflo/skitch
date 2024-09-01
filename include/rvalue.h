#ifndef __SKRVALUE__
#define __SKRVALUE__
#include <string>
#include "globalsettings.h"
#include "value.h"
#include <list>
#include <stack>
using namespace std;

class SKModel;
class SKEntity;
class SKAction;
class SKModel;

class SKRValue : public SKValue
{
    protected:
        SKModel *_model;
    public:
        SKRValue(SKModel *m) {_model = m;}
        virtual ~SKRValue() {}
        virtual float GetValue() = 0;
        virtual void GetValue(stack<float>&s)
        {
            s.push(GetValue());
        }

        virtual string DebugString() {return "";}
};

class SKSingleRValue : public SKRValue
{
    private:
        string _varName;
        float _rawValue;
        bool _isDigit;
        bool _applyNegative;
      
    public:
        SKSingleRValue(string varName, SKModel *m, bool applynegative);
        SKSingleRValue(float value, SKModel *m);
        virtual ~SKSingleRValue() {}

        SKValue *AllocCopy() override
        {
            SKSingleRValue *returnvalue = new SKSingleRValue(_varName, _model, _applyNegative);
            returnvalue->_rawValue = _rawValue;
            returnvalue->_isDigit = _isDigit;
            return returnvalue;
        }

        void ReplaceValue(float f);

        float GetValue() override;
        string DebugString() override
        {
            return GetVarName() + ": " + to_string(GetValue());
        }

        
        bool IsDigit() {return _isDigit;}
        
        string GetVarName() {return _varName;}
        
    private:
        bool IsDigit(string var);
};

class SKStateRValue : public SKRValue
{

};

class SKOperationRValue : public SKRValue
{
    public:
        enum class SKStackOperation
        {
            StackPlus,
            StackMinus,
            StackDivide,
            StackMultiply,
            StackModulus,
            StackSin,
            StackCos,
            StackSign,
            StackAbs
        };

        SKStackOperation _operation;
        float _cachedValue;

    public:
        SKOperationRValue(SKModel *m, SKStackOperation op);
        SKValue* AllocCopy() override;
        float GetValue() override;
        void GetValue(stack<float>& stack) override;
        bool BinaryOperation(SKStackOperation op);
        string DebugString() override
        {
            return string("Operation: ") + to_string((int)_operation);
        }
};

class SKEquationRValue : public SKRValue
{
    private:
        list<SKRValue *> _values;
        
    public:
        SKEquationRValue(SKModel *m, list<SKRValue *> values);
        virtual ~SKEquationRValue() {}
        SKRValue* AllocCopy() override;

        float GetValue() override;
};

#endif