#include "fractaltransformer.h"
#include "face.h"
#include "point.h"
#include "model.h"
#include "rvalue.h"
#include "context.h"
#include <random>

 SKFractalTransformer::~SKFractalTransformer()
 {

 }

 SKPoint *SKFractalTransformer::RiffPoint(SKModel* model, SKPoint *tp, Vector4 point)
 {
    std::mt19937_64 engine(std::random_device{}());
    std::uniform_real_distribution<float> dist(_low, _high);
    float adjust = dist(engine);

    SKSingleRValue x(point.x, model); SKSingleRValue *xx = (SKSingleRValue *)model->AddValue(&x);
    SKSingleRValue y(point.y, model); SKSingleRValue *yy = (SKSingleRValue *)model->AddValue(&y);
    SKSingleRValue z(point.z+adjust, model); SKSingleRValue *zz = (SKSingleRValue *)model->AddValue(&z);

    SKPoint *riff = tp->Duplicate(model); //Ensures all translations are consistent with other points

    riff->SetX(xx); riff->SetY(yy); riff->SetZ(zz);
    return riff;
 }

 SKFace *CreateFaceWithPoints(SKPoint *a, SKPoint *b, SKPoint *c, SKColorVariable *border, SKColorVariable *fill, SKModel *model, Vector4 normal)
 {
    SKFace *returnvalue = NULL;

    SKFace f1(a, b, c, border, fill);
    f1.ComputeCenter(); 
    f1.ComputeNormal();
    Vector4 n1 = f1.GetNormal();
    if(n1.Dot(normal)>=0)
        returnvalue = model->AddFace(&f1);
    else
    {
        SKFace f2(c, b, a, border, fill);
        returnvalue = model->AddFace(&f2);
    }    
    return returnvalue;
 }
 
 //TODO: Clean up the points that are no longer needed (new points created for temp faces)
 //TODO: Ensure that the new faces are pointing in roughly the same direction as the previous faces

void SKFractalTransformer::Transform(SKModel *model, SKShape *shape)
{
    int iterations = ((SKSingleRValue *)_params[0])->GetValue();
    _low = ((SKSingleRValue *)_params[1])->GetValue();
    _high = ((SKSingleRValue *)_params[2])->GetValue();

    for(int i=0;i<iterations;i++)
    {
        list<SKFace *> faces;
        shape->GetFaces(faces);
        vector<SKFace *> remove;
        vector<SKFace *> add;

        for(auto a: faces)
        {
            a->ComputeCenter();
            a->ComputeNormal();
            SKPoint *pA, *pB, *pC;
            a->GetPoints(&pA, &pB, &pC, 1);
            Vector4 normal = a->GetNormal();
            Vector4 center = a->GetCenter();

            SKPoint *cp = RiffPoint(model, pA, center);
            add.push_back(CreateFaceWithPoints(pB, cp, pA, a->GetBorderColor(), a->GetFillColor(), model, normal));
            add.push_back(CreateFaceWithPoints(cp, pC, pA, a->GetBorderColor(), a->GetFillColor(), model, normal));
            add.push_back(CreateFaceWithPoints(pC, cp, pB, a->GetBorderColor(), a->GetFillColor(), model, normal));
            remove.push_back(a);
            shape->ManagePoint(cp);
        }

        // Remove the faces from the model
        for(auto a: remove)
            model->RemoveFace(a);

        // Add the new faces to the shape
        shape->ReplaceFaces(add);
    }
}