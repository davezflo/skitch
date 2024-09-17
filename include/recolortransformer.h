#ifndef __RECOLOR__
#define __RECOLOR__

#include "transformer.h"

class SKRecolorTransformer : public SKTransformer
{
    public:
        SKRecolorTransformer() {};
        virtual ~SKRecolorTransformer();
        SKRecolorTransformer *Create() override {return new SKRecolorTransformer();} 
        void Transform(SKModel *model, SKShape *shape) override;
};

#endif