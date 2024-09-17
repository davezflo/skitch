#ifndef __FRACTAL__
#define __FRACTAL__

#include "transformer.h"

class SKPoint;
class SKFractalTransformer : public SKTransformer
{
    private:
        float _low, _high;

    public:
        SKFractalTransformer() {_low = -0.1, _high = 0.1;}
        virtual ~SKFractalTransformer();
        SKFractalTransformer *Create() override {return new SKFractalTransformer();} 
        void Transform(SKModel *model, SKShape *shape) override;

    private:
        SKPoint *RiffPoint(SKModel* model, SKPoint *tp, Vector4 point);
        
};

#endif