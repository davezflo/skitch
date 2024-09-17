#include <string>
#include <map>
#include <list>
#include <vector>
using namespace std;

#ifndef __ENTITY__
#define __ENTITY__

#define rx "rx"
#define tx "tx"
#define ry "ry"
#define ty "ty"
#define rz "rz"
#define tz "tz"
#define lx "lx"
#define ly "ly"
#define lz "lz"
#define pa "pa"
#define sc "sc"

class SKProjection;
class SKAction;
class SKAttributeVariable;
class SKModel;
class SKContext;
class SKEntity
{
    private:
        typedef struct _ReadyAction
        {
            SKAction *action;
            vector<float> parameters;
        } ReadyAction;

    private:
        list<SKAction *> _retiredActions;
        map<string, list<void(*)(SKEntity *, string, float)>> _registeredCallbacks;
        list<SKAction *> _actions;
        map<string, list<ReadyAction>> _actionQueue;
        map<string, SKAction *> _runningActions;
        string _name;
        bool _needsRender;

    protected:
        SKModel *_model;
        map<string, SKAttributeVariable *> _attributes;
        map<string, string> _tags;
        
   
    public:
        SKEntity(SKModel *model);
        virtual ~SKEntity();
        virtual void Update(SKProjection *projection);
        void Attach(SKAction *action, vector<float> parameters, string waiton);
        void Detach(SKAction *action);
        string GetName();
        SKModel *GetModel() {return _model;}

        SKAttributeVariable *GetAttributeVariable(string member);
        virtual void SetAttribute(string member, float value);
        virtual float GetAttribute(string member);
        virtual bool CheckAttribute(string attr);
        virtual void ResetNeedsRender(bool render) {_needsRender = render;}
        virtual string DebugString();
        void SetTag(string tag, string value) {_tags[tag] = value;}
        string GetTag(string tag) {return _tags[tag];}
        bool NeedsRender() {return _needsRender;}
        
    protected:
       void SetName(string name);
    
    private:
        void RegisterAndRun(SKAction *action, vector<float> parameters, string waiton);

    public:
        void RegisterPropertyCallback(string member, void(*callback)(SKEntity *, string, float));

};

#endif
