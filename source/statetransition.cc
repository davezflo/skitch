#include "statetransition.h"
#include "exceptions.h"
#include "eventlistener.h"
#include "statevar.h"
#include "globalsettings.h"
#include "entity.h"
#include "action.h"
#include "model.h"
#include "context.h"
#include "maths.h"

void SKStateTransition::PrepareForAction(SKAction *action, string parentState)
{
    
}

bool SKStateTransition::Evaluate(SKEntity *entity, string& nextstate)
{
    bool returnvalue = false;
    SKStateVariable *var = (SKStateVariable *)entity->GetModel()->GetCurrentContext()->GetVariable(_stateVariable);
    if(_matchVarState == "") 
    {
        nextstate = _nextState;
        var->SetValue(_nextVarState);
        returnvalue = true; //Otherwise
    }
    else
    {
        if(_matchVarState == var->GetValue())
        {
            var->SetValue(_nextVarState);
            nextstate = _nextState;
            returnvalue = true;
        }
    }
    return returnvalue;
}
