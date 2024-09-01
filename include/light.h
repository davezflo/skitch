#ifndef __LIGHT__
#define __LIGHT__

#include "entity.h"

#include "entity.h"
#include "projection.h"

class SKModel;
class SKFace;
class SKLight : public SKEntity
{
    private:
        SKModel *_model;
        Vector4 _currentLightPosition;
        float _fiftyPercentDistance;
        float _throw;
        bool _changed;
    public:
        SKLight(SKModel *model);
        virtual ~SKLight();

        void Reset() {_changed = false;}
        bool Changed() {return _changed;}

        void SetAttribute(string member, float value) override;

        void SetTx(float distance);
        void SetTy(float distance);
        void SetTz(float distance);
        float GetThrow() {return _throw;}

        Vector4 GetLocation();
        Vector4 GetProjectedLocation();
};

#endif