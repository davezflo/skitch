#include "comptransition.h"
#include "exceptions.h"
#include "lvalue.h"
#include "eventlistener.h"
#include "rvalue.h"
#include "globalsettings.h"
#include "entity.h"
#include "action.h"
#include "maths.h"

void SKCompTransition::PrepareForAction(SKAction *action, string parentState)
{
    if(!_lValue || !_rValue) return;
    if(_lValue && MemberCheck::FindMemberType(_lValue->GetVarName())==MemberType::KeyMember)
    {
        _eventBased = true;
        string key = MemberCheck::GetVarMember(_lValue->GetVarName());
        string state = ((SKSingleRValue *)_rValue)->GetVarName();
        action->RegisterForKeyEvent(key, state, parentState, _nextState);
    }
    else if(_lValue && MemberCheck::FindMemberType(_lValue->GetVarName())==MemberType::MouseMember)
    {
        _eventBased = true;
        string mkey = MemberCheck::GetVarMember(_lValue->GetVarName());
        MouseState state = MouseState::LeftButton;
        if(mkey == "right") state = MouseState::RightButton;
        action->RegisterForMouseEvent(state, parentState, _nextState);
    }
}

bool SKCompTransition::Evaluate(SKEntity *entity, string& nextstate)
{
    if(_eventBased) return false;
    if(_lValue == NULL && _rValue == NULL) 
    {
        nextstate = _nextState;
        return true; //Otherwise
    }

    bool returnvalue = false;
    nextstate = _nextState;
    float lval = _lValue->GetValue();
    float rval = _rValue->GetValue();

    switch(_comp)
    {
        case Comparison::Equal:
            returnvalue = myround(lval)==myround(rval)?true:false;
            break;
        case Comparison::NotEqual:
            returnvalue = myround(lval)!=myround(rval)?true:false;
            break;
        case Comparison::GTOrEqual:
            returnvalue = myround(lval)>=myround(rval)?true:false;
            break;
        case Comparison::LTOrEqual:
            returnvalue = myround(lval)<=myround(rval)?true:false;
            break;
    }

    return returnvalue;
}

// void SKTransition::CheckTransition(list<string> validstates, SKEntity *entity)
// {
//     if(_lValue != NULL) _lValue->CheckVariable(entity);
//     if(_rValue != NULL) _rValue->CheckVariable(entity);
//     bool failed = true;
//     for(auto it=validstates.begin();it!=validstates.end();++it)
//     {
//         if((*it)==_nextState)
//         {
//             failed = false;
//             break;
//         }
//     }
//     if(failed && _nextState != DEINIT)
//         throw ParseException {_nextState + " state not found in action."};   
// }