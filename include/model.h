#include <map>
#include "variable.h"
#include "point.h"
#include "face.h"
#include "shape.h"
#include "transition.h"
#include "assignment.h"
#include "actionsequence.h"
#include "action.h"
#include "eventlistener.h"
#include "value.h"
#include <list>
#include "surface.h"
#include <map>
#include <chrono>

using namespace std;

class SKProjection;
class SKGenerator;
class SKCamera;
class SKContext;
class SKMap;
class SKPunchTicket;
class SKMapMove;
class SKWatch;
class SKLight;
class SKInteractLight;
class SKVariable;
class SKConsume;
class SKModel
{
    private:
        chrono::high_resolution_clock::time_point _lastTime;
        string _fps;
        vector<SKSurface> _surfaces;
        vector<SKFace*> _allfaces;
        list<SKActionSequence *> _sequences;
        list<SKPoint *> _points;
        list<SKFace *> _faces;
        list<SKShape *> _shapes;
        list<SKWatch *> _watches;
        list<SKAssignment *> _assignments;
        list<SKTransition *> _transitions;
        list<SKGenerator *> _generators;
        list<SKConsume *> _consumers;
        list<SKActionThread *> _actionthreads;
        list<SKValue *> _values;
        list<SKMapMove *> _mapMoves;
        list<SKVariable *> _internalVars;
        list<SKPunchTicket *> _tickets;
        map<string, SKMap *> _maps;
        map<string, SKAction *> _actions;
        map<string, list<EventListener *>> _listeners;
        SKProjection *_projection;
        SKCamera *_camera;
        SKLight *_light;
        SKContext *_globalContext;
        stack<SKContext *> _contextStack;
        Vector4 _renderDimensions;
        bool _needsRender;
        map<string, SKShape *> _liveShapes;
        SKInteractLight *_lightInteractions;

    public:
        SKModel();
        virtual ~SKModel(); 

    public:
        vector<SKShape *> GetShapes();
        SKShape *GetShape(string name);
        SKMap *GetMap(string name);
        SKProjection *GetProjection() {return _projection;}
        SKCamera *GetCamera() {return _camera;}
        SKLight *GetLight() {return _light;}
        void SetRenderDimensions(float x, float y);
        Vector4 GetRenderDimensions();

        SKContext *GetGlobalContext() {return _globalContext;}
        void PushContext(SKContext *);
        SKContext *PopContext();
        SKContext *GetCurrentContext() 
        {
            if(_contextStack.size() > 0)
                return _contextStack.top();
            else 
                return _globalContext;
        }
        SKPoint *AddPoint(SKPoint *point);
        void RemovePoint(SKPoint *point);
        SKFace *AddFace(SKFace *face);
        void RemoveFace(SKFace *face);
        SKShape *AddShape(SKShape *shape);
        SKMapMove *AddMapMove(SKMapMove *move);
        SKMap *AddMap(SKMap *map);
        SKWatch *AddWatch(SKWatch *watch);
        SKAssignment *AddAssignment(SKAssignment *assignment);
        SKTransition *AddTransition(SKTransition *transition);
        SKActionSequence *AddActionSequence(SKActionSequence *seq);
        SKActionThread *AddActionThread(SKActionThread *aThread);
        SKConsume *AddConsumer(SKConsume *consume);
        SKValue *AddValue(SKValue *value);
        SKAction *AddAction(SKAction *action);
        void DeleteAction(SKAction* action);
        SKPunchTicket *AddTicketPunch(SKPunchTicket *ticketpunch);
        SKGenerator *AddGenerator(SKGenerator *generator);
        SKShape *AddShapeInstance(string name, string prototype);
        SKShape *CreateActiveAnonShape(vector<Vector4> points, list<vector<int>> faces, uint32_t fill, uint32_t border);
        void UpdateActiveAnonShape(SKShape *s, vector<Vector4> pointValues);
        void AttachActionToShape(string shape, string action, vector<float> parameters);
        void RegisterKeyListener(string key, KeyState state, EventListener *l);
        void RegisterMouseListener(MouseState, EventListener *l);
        void FireKeyEvent(string key, KeyState state);
        void FireMouseEvent(float x, float y, MouseState state);
        void BuildActionThread(string actionName, SKEntity *entity, string memberVariable, 
                                vector<SKRValue *> values, string waiton, string ticket);
        bool WatchLight() {return true;} //TODO: Fix
        bool WatchPoints();
        void Update();

        void Rendered() {_needsRender = true;}
        bool NeedsRender() 
        {
            bool returnvalue = _needsRender;
            _needsRender = false;
            return returnvalue;
        }

        string GetFPS() {return _fps;}

        vector<SKSurface> GetSurfaces() {return _surfaces;}

        void ProcessLightInteraction(SKShape *s);

    private:
        SKVariable *AddAnonVariable(SKVariable *var);
        void SortAndPrepareSurfaces();
        void AddFaceToSurface(SKFace *face, Vector4 nd);
        void CalculateSurfaces();
        void LogTime();
};
