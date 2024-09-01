#ifndef __INTERACTLIGHT__
#define __INTERACTLIGHT__

#include "maths.h"
#include <list>
#include <vector>
#include <map>

using namespace std;


class SKShape;
class SKFace;
class SKLight;
class SKModel;
class SKInteractLight 
{
    private:
        SKModel *_model;
        map<SKFace *, SKShape *> _generatorMap;

    public:
        SKInteractLight(SKModel *model) {_model = model;}
        void Interact(SKShape *s);
        vector<SKShape *> GetGeneratedShapes();

    private:
        void CreateShadow(SKFace *f, SKLight *light);
        Vector4 CreateShadowPoint(float surfaceY, Vector4 objectPoint, Vector4 light);
};

#endif