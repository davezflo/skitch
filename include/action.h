#include <map>
#include <vector>
#include <string>
#include <deque>
#include "eventlistener.h"
using namespace std;

#ifndef __ACTION__
#define __ACTION__

#define INIT "init"
#define DEINIT "deinit"

class SKActionSequence;
class SKEntity;
class SKModel;
class SKVariable;
class SKContext;
class SKAction : EventListener
{
    private:
        SKContext *_context;
        map<string, SKActionSequence *> _sequences;
        map<string, map<string, string>> _keymap; 
        deque<string> _stateHistory;
        
        vector<string> _variables;
        SKEntity *_attached;
        SKModel *_model;
        string _name;
        string _currentState;
        string _parameter;
        string _ticket;


    public:
        SKAction(SKModel *model, string name, SKContext *initContext);
        virtual ~SKAction();
        string GetName();
        SKContext *GetContext() {return _context;}
        void SetParameter(string value);
        void SetTicket(string ticketname);
        void SetParentContext(SKContext *context);
        SKAction* AllocCopy();
        void Update(SKEntity *s);
        
        void AddActionSequence(SKActionSequence *sequence);
        void AddFormalParameter(string variable) {_variables.push_back(variable);}
     
        void Register(SKEntity *entity, vector<float> parameters);
        void Deregister(SKEntity *entity);

        void KeyEvent(string key, KeyState state) override;
        void MouseEvent(float x, float y, MouseState state) override;
        void RegisterForKeyEvent(string key, string state, string instate, string tostate);
        void RegisterForMouseEvent(MouseState m, string instate, string tostate);

        string DebugString();

    private:
        void UpdateCurrentState(string newstate);
        string NameState() {return _name+"state";}
        void Register(SKEntity *s);
        void InitializeVariables();
        void UpdateStateHistory();
        
};


#endif