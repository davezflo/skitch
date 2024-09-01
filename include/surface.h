#ifndef __SURFACE__
#define __SURFACE__
#include "maths.h"
#include <vector>

class SKFace;
class SKModel;
class SKSurface
{
    Vector4 _normalPoint;
    vector<SKFace *> _faces;
    vector<SKFace *> _renderFaces;
    bool _interacts = false;

    public:
        SKSurface();
        SKSurface(const SKSurface& s);
        void SetNormalPoint(Vector4 np);
        Vector4 GetNormalPoint() {return _normalPoint;}
        void AddFace(SKFace *f);
        bool operator==(const Vector4& s);
        void PrepareForRender(SKModel *model);
        vector<SKFace *> GetRenderedFaces() {return _renderFaces;}
        void SetInteracts(bool interacts) {_interacts = true;}
        bool GetInteracts() {return _interacts;}
        string DebugString();

};

#endif