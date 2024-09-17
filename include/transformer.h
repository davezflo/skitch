#ifndef __TRANSFORMER__
#define __TRANSFORMER__

#include "shape.h"
#include <vector>
#include "rvalue.h"

class SKTransformer 
{
    protected:
        vector<SKRValue *> _params;

    public:
        SKTransformer() {}
        virtual ~SKTransformer();
        virtual SKTransformer *AllocCopy();
        virtual SKTransformer *Create() = 0;
        virtual void Transform(SKModel *model, SKShape *shape) = 0;
        void Initialize(vector<SKRValue *> parameters);
        
};

#endif