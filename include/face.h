#include "colorvar.h"
#include "point.h"
#include "maths.h"
#include <vector>

#ifndef __FACE__
#define __FACE__

class SKProjection;
class SKModel;
class SKLight;
class SKFace
{
    private:
        SKPoint *_pointA;
        SKPoint *_pointB;
        SKPoint *_pointC;
        Vector4 _center;
        Vector4 _translationFreeCenter;
        Vector4 _normal;
        Matrix4x4 _xMatrix, _yMatrix, _zMatrix;
        Matrix4x4 _translation;
      
        float minx, miny, minz, maxx, maxy, maxz;

        SKColorVariable *_border;
        SKColorVariable *_fill;
        SKColorVariable _adjustedBorder;
        SKColorVariable _adjustedFill;
        float _distance;
        int _needsUpdate;

    public:
        SKFace(SKPoint *pA, SKPoint *pB, SKPoint *pC, 
                SKColorVariable *border, 
                SKColorVariable *fill);

        virtual ~SKFace()
        {
           
        }
        
        SKColorVariable *GetBorderColor();
        SKColorVariable *GetFillColor();

        void SetPointValues(Vector4 a, Vector4 b, Vector4 c);
        
        void Update(SKModel *model, vector<string> interacts);
        bool IsFaceVisibleFromThisAngle(SKProjection *p);        
        void GetPoints(SKPoint **pA, SKPoint **pB, SKPoint **pC, float extent);
        SKFace *Duplicate(SKModel *model, vector<SKPoint *> points);
        SKFace *AllocCopy();
        bool Uses(SKPoint *p) {if(p==_pointA||p==_pointB||p==_pointC) {return true;} else {return false;}}
        float Distance() {return _distance;}
        void Rendered() {_needsUpdate--; if(_needsUpdate<0) _needsUpdate=0;}
        bool IsRendered() {return _needsUpdate==0;}
        
        string DebugString(SKModel *model);

        void ComputeCenter();
        void ComputeNormal();
        Vector4 GetNormal() {return _normal;}
        Vector4 GetCenter();

        void ApplyRotationX(float degrees);
        void ApplyRotationY(float degrees);
        void ApplyRotationZ(float degrees);
        void ApplyTranslationX(float value);
        void ApplyTranslationY(float value);
        void ApplyTranslationZ(float value);
        
    private:
        SKPoint *Locate(SKPoint *in, vector<SKPoint *>points);
};

#endif
