#include "maths.h"
#include "globalsettings.h"
#include "rvalue.h"
#ifndef __POINT__
#define __POINT__

class SKModel;
class SKProjection;
class SKPoint
{
    private: 
        SKModel *_model;
        SKRValue *_x;
        SKRValue *_y;
        SKRValue *_z;
        string _name;
        Matrix4x4 _xMatrix, _yMatrix, _zMatrix;
        Matrix4x4 _translation;
        Vector4 _lastVec;
        Vector4 _lastVecNoProjection;
        float _extent;
        bool _template;

    public: 
        SKPoint(string name, SKModel *model) 
        {
            _extent = 100;
            _name = name;
            _model = model;
            _template = true;
        }

        virtual ~SKPoint()
        {
        }
        SKPoint *AllocCopy()
        {
            SKPoint *returnvalue = new SKPoint(_name, _model);
            returnvalue->SetX(_x);
            returnvalue->SetY(_y);
            returnvalue->SetZ(_z);
            return returnvalue;
        }
        void RemoveTemplate() {_template = false;}
        bool IsTemplate() {return _template;}
        SKPoint *Duplicate(SKModel *model);
        string GetName() {return _name;}
        string DebugString();
        void SetX(SKRValue *x) {_x = x; _lastVecNoProjection.x = x->GetValue();}
        void SetY(SKRValue *y) {_y = y; _lastVecNoProjection.y = y->GetValue();}
        void SetZ(SKRValue *z) {_z = z; _lastVecNoProjection.z = z->GetValue();}

        void SetExtent(float extent) {_extent = extent;}

        float GetRawX() {return _x->GetValue();}
        float GetRawY() {return _y->GetValue();}
        float GetRawZ() {return _y->GetValue();}

        float GetX() {return _extent*_lastVec.x;}
        float GetY() {return _extent*_lastVec.y;}
        float GetZ() {return _extent*_lastVec.z;}

        SKRValue *GetXRValue() {return _x;}
        SKRValue *GetYRValue() {return _y;}
        SKRValue *GetZRValue() {return _z;}

        void Live() {_template = false;}

        Vector4 GetUnProjectedVector() 
        {
            return Vector4(_extent*_lastVecNoProjection.x,
                            _extent*_lastVecNoProjection.y,
                            _extent*_lastVecNoProjection.z,
                            1);
        }
        Vector4 GetRawUnProjectedVector() 
        {
            return Vector4(_lastVecNoProjection.x,
                            _lastVecNoProjection.y,
                            _lastVecNoProjection.z,
                            1); 
        }

        Vector4 Cross(const SKPoint& otherpoint);
        Vector4 Minus(const SKPoint& otherpoint);
        float Dot(const SKPoint& otherpoint);

        void ApplyRotationX(float degrees);
        void ApplyRotationY(float degrees);
        void ApplyRotationZ(float degrees);
        void ApplyTranslationX(float value);
        void ApplyTranslationY(float value);
        void ApplyTranslationZ(float value);

        void Update(SKProjection* projection);
};

#endif
