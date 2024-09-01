#ifndef __COMPTRANSITION__
#define __COMPTRANSITION__
#include <string>
#include <list>
#include "transition.h"
using namespace std;

enum class Comparison
{
    Equal,
    NotEqual,
    GTOrEqual,
    LTOrEqual
};

class SKLValue;
class SKRValue;
class SKEntity;
class SKAction;
class SKCompTransition : public SKTransition
{
    private:
        SKLValue *_lValue;
        SKRValue *_rValue;
        Comparison _comp;
        string _nextState;
        bool _eventBased;
       
    public:
        SKCompTransition()
        {
            _comp = Comparison::Equal;
            _nextState = "";
            _lValue = NULL;
            _rValue = NULL;
            _eventBased = false;
        }
        virtual ~SKCompTransition() {}

        SKTransition *AllocCopy()
        {
            SKCompTransition *returnvalue = new SKCompTransition();
            returnvalue->SetLValue(_lValue);
            returnvalue->SetRValue(_rValue);
            returnvalue->SetComparison(_comp);
            returnvalue->SetNextState(_nextState);
            return returnvalue;
        }
        void PrepareForAction(SKAction *action, string parentState);
        bool Evaluate(SKEntity *entity, string& nextState);
        
        void SetNextState(string nextState) {_nextState = nextState;}
        void SetLValue(SKLValue *value) {_lValue = value;}
        void SetRValue(SKRValue *value) {_rValue = value;}
        void SetComparison(Comparison comp) {_comp = comp;}
        

        //void CheckTransition(list<string> validstates, SKEntity *entity);
};


#endif