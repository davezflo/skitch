#ifndef __ATTRVAR__
#define __ATTRVAR__

#include "numvar.h"

class SKEntity;
class SKAttributeVariable : SKNumberVariable
{
     private:
        SKEntity *_parent;

    public:
        SKAttributeVariable(string name, SKEntity *parent);

        SKVariable *AllocCopy() override;
        void SetValue(float value);
        void UpdateValue(float value) {_value = myround(value);}
        float GetValue() override;
};


#endif