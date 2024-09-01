#include "parseaction.h"
#include "context.h"
#include "model.h"
#include "action.h"
#include "actionsequence.h"
#include "actionthread.h"
#include "comptransition.h"
#include "statetransition.h"
#include "numvar.h"
#include "exceptions.h"
#include "ticket.h"
#include "lvalue.h"
#include "consume.h"

bool SKParseAction::IsTransition(string name)
{
    bool returnvalue = (name == StateTransTok)?true:false;
    return returnvalue;
}

bool SKParseAction::IsConsume(string name)
{
    bool returnvalue = (name == ConsumeTok)?true:false;
    return returnvalue;
}

Comparison SKParseAction::ToCompare(string token)
{
    if(token == EqTok) return Comparison::Equal;
    if(token == NeqTok) return Comparison::NotEqual;
    if(token == GteTok) return Comparison::GTOrEqual;
    if(token == LteTok) return Comparison::LTOrEqual;
    throw ParseException("Illegal comparison token: " + token);
}

Operation SKParseAction::ToOperation(string token)
{
    if(token == AssignTok) return Operation::Assign;
    if(token == PlusAssignTok) return Operation::Plus;
    if(token == MinusAssignTok) return Operation::Minus;
    if(token == DivideAssignTok) return Operation::Divide;
    if(token == TimesAssignTok) return Operation::Times;
    throw ParseException("Illegal operation token: " + token);
}

bool SKParseAction::IsCompare(string token)
{
    bool returnvalue = false;
    if(token == EqTok ||
        token == NeqTok ||
        token == GteTok ||
        token == LteTok)
        returnvalue = true;
    return returnvalue;
}

bool SKParseAction::IsAssign(string token)
{
    bool returnvalue = false;
    if(token == AssignTok ||
        token == DivideAssignTok ||
        token == PlusAssignTok ||
        token == TimesAssignTok ||
        token == MinusAssignTok)
        returnvalue = true;
    return returnvalue;
}

SKActionThread *SKParseAction::ExtractActionThread(SKToken **check, SKContext *context)
{
    SKActionThread *returnvalue = NULL;
    ExtractOnMatch(ThreadOpenTok, check);
    string actionname = ExtractOnMatch(VarTok, check);
    string member = "";
    string token = ExtractNextToken(check, true, member);
    vector<SKRValue *> values;
    while(PeekNextToken(*check) != ThreadCloseTok)
    {
        SKRValue *var = ExtractRValue(check, context);
        values.push_back(var);
    }
    
    *check = (*check)->next;
    string waiton = "";
    string ticket = "";
    if(PeekNextToken(*check) == WaitTok)
        waiton = ExtractOnMatch(WaitTok, check).substr(1);
    
    if(PeekNextToken(*check) == TicketTok)
        ticket = ExtractOnMatch(TicketTok, check).substr(1);
    
    SKActionThread th(actionname, 
        MemberCheck::FindMemberType(member),
        MemberCheck::GetVarMember(member), values, 
        waiton, ticket);
    returnvalue = _model->AddActionThread(&th);
 
    return returnvalue;
}

SKToken *SKParseAction::Parse(SKToken *token, SKContext *context, map<string, SKElementParser *> parsers)
{
    SKToken *check = token;
    string actionName = ExtractOnMatch(VarTok, &check);
    ExtractOnMatch(OpenTok, &check);
    bool done = false;
    
    SKContext actionContext(actionName + " context", _model);
    list<string> variablenames;
    
    /* First extract the variables declared in the action */
    //TODO: We need to add state variables to this
    while(PeekNextToken(check)==NumTok)
    {
        ExtractOnMatch(NumTok, &check);
        string varname;
        check = parsers[NumTok]->Parse(check, &actionContext, parsers);
        variablenames.push_back(actionContext.GetScratchString(LASTVAR));
        check = check->prev;
    }
    
    /* Init is next */
    ExtractOnMatch(InitStateTok, &check);

    list<SKActionSequence *> _sequences;
    list<string> names;
    
    string m1, m2, m3, n1, n2, n3;    
    SKActionSequence *current = new SKActionSequence(_model, INIT);
    names.push_back(INIT);
    
    while(!done)
    {
        /* Inside a state definition, we have action threads, assignments, transitions, and ticket management */
        while(PeekNextToken(check) == ThreadOpenTok)
        {
            SKActionThread *t = ExtractActionThread(&check, &actionContext);
            if(t) current->AddActionThread(t);
        }    

        n1 = ExtractNextToken(&check, false, m1);
        n2 = ExtractNextToken(&check, true, m2);

        if(n1 == PunchTok && n2 == VarTok)
        {
            SKNumberVariable v(m2);
            v.SetValue(0.0);
            actionContext.AddVariable(&v);
            SKPunchTicket p(m2, _model);
            SKPunchTicket *pt = _model->AddTicketPunch(&p);
            current->AddTicketPunch(pt);
        }
        else if(n2 == QueryTok)
        {
            SKStateTransition t;
            n3 = ExtractNextToken(&check, false, m3);
            string matchstate = "";
            if(n3 == VarTok)
                matchstate = m3;
            else if(n3 != OtherTok)
                throw ParseException{"Expecting VarTok or OtherwiseTok."};

            ExtractOnMatch(StateTransTok, &check);
            string nextvarstate = ExtractOnMatch(VarTok, &check);
            ExtractOnMatch(CombineTok, &check);
            string nextstate = ExtractOnMatch(VarTok, &check);

            t.SetStateVariable(m1);
            t.SetMatchState(matchstate);
            t.SetNextVarState(nextvarstate);
            t.SetNextState(nextstate);
            current->AddTransition(_model->AddTransition((SKTransition*)&t));
        }
        else
        {
            SKLValue lvalue(m1, _model);
            
            if(IsAssign(n2))
            {
                SKAssignment assign;
                SKRValue *rvalue = ExtractEquation(&check, &actionContext);
                assign.SetOperation(ToOperation(n2));
                assign.SetLValue((SKLValue *)_model->AddValue((SKValue *)&lvalue));
                assign.SetRValue(rvalue);
                current->AddAssignment(_model->AddAssignment(&assign));
            }
            else if(IsConsume(n2))
            {
                SKConsume consume;
                n3 = ExtractNextToken(&check, true, m3);
                if(n3 == PointTok)
                {
                    SKRValue *x, *y, *z;
                    x = ExtractRValue(&check, &actionContext);
                    y = ExtractRValue(&check, &actionContext);
                    z = ExtractRValue(&check, &actionContext);
                    consume.ConsumePoint(x, y, z);
                }
                else if(n3 == FaceTok)
                {
                    string border = ExtractOnMatch(VarTok, &check); //TODO: verify this is a color
                    string fill = ExtractOnMatch(VarTok, &check); //TODO: verify this is a color
                    consume.ConsumeFace(border, fill);
                }
                else if(n3 == MouseMemberTok)
                {
                    if(m3 != "m.np") //TODO: could do last point here too
                        throw ParseException("Only valid mouse value is nearest point.");
                    consume.ConsumeMousePoint();
                }
                else if(n3 == DivideTok)
                {
                    consume.ConsumeUndo();
                }
                else
                    throw ParseException("Invalid Consumption");

                current->AddConsumer(_model->AddConsumer(&consume));
            }
            else if(IsCompare(n2))
            {
                SKCompTransition trans;
                trans.SetComparison(ToCompare(n2));
                trans.SetLValue((SKLValue *)_model->AddValue((SKValue *)&lvalue));
                SKRValue *rvalue = ExtractRValue(&check, &actionContext);
                trans.SetRValue(rvalue);
                ExtractOnMatch(StateTransTok, &check);
                trans.SetNextState(ExtractOnMatch(VarTok, &check));
                current->AddTransition(_model->AddTransition(&trans));
            }
            else if(n1 == OtherTok && IsTransition(n2))
            {
                SKCompTransition trans;
                n3 = ExtractNextToken(&check, true, m3);
                trans.SetNextState(m3);
                current->AddTransition(_model->AddTransition((SKTransition*)&trans));
            }
        }
        
        if(PeekNextToken(check) == CloseTok)
        {
            if(current != NULL)
            {
                _sequences.push_back(_model->AddActionSequence(current));
                delete(current); 
            }
            
            SKAction action(_model, actionName, &actionContext);
            
            for(auto v:variablenames)
                action.AddFormalParameter(v);
            list<float> parameters;
            
            for(auto it=_sequences.begin();it!=_sequences.end();++it)
                action.AddActionSequence((*it));
            ExtractOnMatch(CloseTok, &check);
            _model->AddAction(&action);
            done = true;
        }
        else if(PeekNextToken(check) == StateDefTok)
        {
            string name = ExtractOnMatch(StateDefTok, &check);
            name = name.substr(0, name.find(":"));
            names.push_back(name);
            _sequences.push_back(_model->AddActionSequence(current));
            delete(current);
            current = new SKActionSequence(_model, name);
        }
    }

    check = check->next;
    return check;
}