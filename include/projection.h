#ifndef __PROJECTION__
#define __PROJECTION__
#include "maths.h"

class SKModel;
class SKProjection
{
    private:
        Matrix4x4 _xMatrix;
        Matrix4x4 _yMatrix;
        Matrix4x4 _zMatrix;
        Matrix4x4 _attentionMatrix;
        Vector4 _orientation;

        float _adjustY;
        float _adjustX;
        float _scalingFactor = 1;
        Vector4 _eye;
        Vector4 _lookat;
        SKModel *_model;
        float _pi;
        Vector4 _negRange;
        Vector4 _posRange;
        Vector4 _direction;
        int _needsUpdate;

    private:
        void UpdateAttention();

    public:
        SKProjection(SKModel *engine);
        virtual ~SKProjection();
        void InitializeDimensions();
        Vector4 Compute(const Vector4& in);
        void TranslateX(float distance);
        void TranslateY(float distance);
        void TranslateZ(float distance);
        void RotateX(float degrees);
        void RotateY(float degrees);
        void RotateZ(float degrees);
        void LookAtX(float location);
        void LookAtY(float location);
        void LookAtZ(float location);
        void Update();
        void LookAt(float x, float y, float z);
        bool PositiveZ() {return _direction.z >= 0;}
        Vector4 ViewVector() {return _direction;}
        Vector4 ReverseCompute(const Vector4& in, float zplane);
        
        Vector4& Eye()
        {
            return _eye;
        }
        Vector4& Orientation() {return _orientation;}
        bool PointVisible(Vector4 in);
        
        void Rendered() {_needsUpdate--; if(_needsUpdate<0) _needsUpdate=0;}
        bool IsRendered() {return _needsUpdate==0;}
};


#endif
