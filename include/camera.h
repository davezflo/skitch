#ifndef __CAMERA__
#define __CAMERA__

#include "entity.h"
#include "projection.h"

class SKModel;
class SKCamera : public SKEntity
{
    private:
        SKProjection *_projection;
        SKModel *_model;
    public:
        SKCamera(SKProjection *projection, SKModel *model);
        virtual ~SKCamera();

        void SetAttribute(string member, float value) override;

        void SetRx(float degrees);
        void SetRy(float degrees);
        void SetRz(float degrees);
        void SetTx(float distance);
        void SetTy(float distance);
        void SetTz(float distance);
        void SetLx(float distance);
        void SetLy(float distance);
        void SetLz(float distance);

        Vector4 GetLocation() 
        {
            return Vector4(GetAttribute(tx),
                            GetAttribute(ty),
                            GetAttribute(tz),
                            1);
        }

        string DebugString() override;
        
};

#endif