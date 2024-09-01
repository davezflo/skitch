#include "actionsequence.h"
#include <string>
#include "shape.h"
#include "assignment.h"
#include "transition.h"
#include "actionthread.h"
#include "action.h"
#include "model.h"
#include "consume.h"
#include "log.h"
#include "ticket.h"
using namespace std;

SKActionSequence::SKActionSequence(SKModel *model, string name)
{
    _model = model;
    _name = name;
}

SKActionSequence::~SKActionSequence()
{

}

string SKActionSequence::GetName() 
{
    return _name;
}

void SKActionSequence::AddAssignment(SKAssignment* assignment)
{
    _assignments.push_back(assignment);
}

void SKActionSequence::AddTransition(SKTransition *transition)
{
    _transitions.push_back(transition);
}

void SKActionSequence::AddActionThread(SKActionThread *thread)
{
    _threads.push_back(thread);
}

void SKActionSequence::AddTicketPunch(SKPunchTicket *punches)
{
    _ticketpunches.push_back(punches);
}

void SKActionSequence::AddConsumer(SKConsume *consume)
{
    _consumers.push_back(consume);
}

SKActionSequence *SKActionSequence::AllocCopy()
{
    SKActionSequence *returnvalue = new SKActionSequence(_model, _name);
    for(auto a : _assignments)
        returnvalue->_assignments.push_back(a);
    for(auto a: _transitions)
        returnvalue->_transitions.push_back(a);
    for(auto a: _threads)
        returnvalue->_threads.push_back(a);
    for(auto a: _ticketpunches)
        returnvalue->_ticketpunches.push_back(a);
    for(auto a: _consumers)
        returnvalue->_consumers.push_back(a);
    return returnvalue;
}

void SKActionSequence::PrepareForAction(SKAction *action)
{
    _parent = action;
    for(auto it=_transitions.begin();it!=_transitions.end();++it)
        (*it)->PrepareForAction(action, GetName());
}

string SKActionSequence::Update(SKEntity *attached, SKContext *context, string ticket)
{
    string returnvalue = _name;
    string newstate;
    for(auto a :_assignments)
        a->Operate(attached);

    for(auto a: _consumers)
        a->Consume(attached);

    for(auto a: _ticketpunches)
        a->Punch();

    //Check own ticket
    if(SKCheckTicket::Check(ticket, _model))
        returnvalue = DEINIT;
    else 
    {
        for(auto a : _transitions)
        {
            if(a->Evaluate(attached, newstate))
            {
                returnvalue = newstate;
                break;
            }
        }
        for(auto a :_threads)
            a->Execute(_model, attached);
    }

    return returnvalue;
}