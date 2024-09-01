#ifndef __STATETRANSITION__
#define __STATETRANSITION__
#include <string>
#include <list>
#include "transition.h"
using namespace std;

class SKLValue;
class SKRValue;
class SKEntity;
class SKAction;
class SKStateTransition : public SKTransition
{
    private:
        string _stateVariable;
        string _matchVarState;
        string _nextVarState;
        string _nextState;
       
    public:
        SKStateTransition()
        {
            _stateVariable = "";
            _matchVarState = "";
            _nextVarState = "";
            _nextState = "";
        }
        virtual ~SKStateTransition() {}

        SKTransition *AllocCopy()
        {
            SKStateTransition *returnvalue = new SKStateTransition();
            returnvalue->SetStateVariable(_stateVariable);
            returnvalue->SetMatchState(_matchVarState);
            returnvalue->SetNextVarState(_nextVarState);
            returnvalue->SetNextState(_nextState);
            return returnvalue;
        }
        void PrepareForAction(SKAction *action, string parentState);
        bool Evaluate(SKEntity *entity, string& nextState);
        
        void SetNextState(string nextState) {_nextState = nextState;}
        void SetStateVariable(string value) {_stateVariable = value;}
        void SetMatchState(string value) {_matchVarState = value;}
        void SetNextVarState(string state) {_nextVarState = state;}
        

        //void CheckTransition(list<string> validstates, SKEntity *entity);
};


#endif