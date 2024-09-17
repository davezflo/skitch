#include "transformer.h"

SKTransformer::~SKTransformer()
{
    
}

SKTransformer *SKTransformer::AllocCopy()
{
    SKTransformer *t = this->Create();
    t->Initialize(this->_params);
    return t;
}

void SKTransformer::Initialize(vector<SKRValue *> parameters)
{
    for(auto a: parameters)
    {
        _params.push_back(a);
    }
}