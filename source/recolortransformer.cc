#include "recolortransformer.h"
#include "shape.h"
#include "face.h"
#include "model.h"
#include "context.h"
#include <list>

using namespace std;

//Sole, expected parameter is a color variable

 SKRecolorTransformer::~SKRecolorTransformer()
 {

 }

void SKRecolorTransformer::Transform(SKModel *model, SKShape *shape)
{
    SKSingleRValue *border = (SKSingleRValue *)_params[0];
    SKSingleRValue *fill = _params.size()>1?(SKSingleRValue *)_params[1]:border;
    SKColorVariable *bc = (SKColorVariable *)model->GetGlobalContext()->GetVariable(border->GetVarName());
    SKColorVariable *fc = (SKColorVariable *)model->GetGlobalContext()->GetVariable(fill->GetVarName());

    list<SKFace *> faces;
    shape->GetFaces(faces);
    for(auto a: faces)
    {
        a->SetBorderColor(bc);
        a->SetFillColor(fc);
    }
}