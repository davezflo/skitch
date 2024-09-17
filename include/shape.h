#include "point.h"
#include "face.h"
#include "maths.h"
#include "entity.h"
#include <vector>
#include <string>

#ifndef __SHAPE__
#define __SHAPE__

class SKAction;
class SKProjection;
class SKModel;
class SKShape : public SKEntity
{
    vector<SKPoint *> _points;
    vector<SKFace *> _faces;
    vector<string> _interacts;
    vector<SKPoint *> _faceCandidate;
    
    protected:
        void SetRx(float degrees);
        void SetRy(float degrees);
        void SetRz(float degrees);
        void SetTx(float distance);
        void SetTy(float distance);
        void SetTz(float distance);
        void SetScale(float distance);

    public:
        SKShape(vector<SKPoint *>points, vector<SKFace *> faces, string name, SKModel *model);
        virtual ~SKShape() {}

        void ManagePoint(SKPoint *point);
        void AddPoint(SKPoint *point);
        void SelectPoint(SKPoint *point);
        void SelectPointFromXY(float x, float y);
        void CreateCandidateFace(string bordervar, string fillvar);
        void UndoLastOperation();

        void Update(SKProjection *) override;

        Vector4 GetCenter();

        void SetShadowFace(Vector4 a, Vector4 b, Vector4 c);

        void SetAttribute(string member, float value) override;
        void GetFaces(list<SKFace *>& faces);
        const vector<SKPoint *>& GetPoints() const {return _points;}
        const vector<SKPoint *>& GetSelectedPoints() const {return _faceCandidate;}
        SKShape *Duplicate(SKModel *model, string name);
        SKShape* AllocCopy();
        void AddInteraction(string interaction);
        void ReplaceFaces(vector<SKFace *>newFaces);
        void AdjustShape(Vector4 newCenter);
        bool Interacts(const string &s)
        {
            bool returnvalue = false;
            for(auto a: _interacts)
                returnvalue |= (a==s);
            return returnvalue;
        }
 
    private:
        void DuplicateShape(SKModel *model);
        bool ProcessLight();
        
};


#endif
