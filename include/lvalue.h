#ifndef __SKLVALUE__
#define __SKLVALUE__
#include <string>
#include "globalsettings.h"
#include "value.h"
using namespace std;

class SKModel;
class SKEntity;
class SKAction;
class SKModel;
class SKLValue : SKValue
{
    private:
        string _varName;
        SKModel *_model;
      
    public:
        SKLValue(string varName, SKModel *m);

        virtual ~SKLValue() {}

         SKValue *AllocCopy() override
        {
            SKLValue *returnvalue = new SKLValue(_varName, _model);
            return returnvalue;
        }
        
        float GetValue();
        void SetValue(float value);
        string GetVarName() {return _varName;}

};

#endif