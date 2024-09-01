#include "elementparser.h"
#include "exceptions.h"
#include "numvar.h"
#include "context.h"
#include "rvalue.h"
#include "model.h"

string SKElementParser::ExtractNextToken(SKToken **check, bool neggreedy, string& matchvalue)
{
    string returnvalue = (*check)->next->Name();
    if(returnvalue == NegTok && neggreedy)
    {
        *check = (*check)->next;
        returnvalue = (*check)->next->Name();   
        matchvalue = "-" + (*check)->next->MatchedValue();
    }
    else
       matchvalue = (*check)->next->MatchedValue();
    *check = (*check)->next;
    return returnvalue;
}

string SKElementParser::PeekNextToken(SKToken *token)
{
    return token->next->Name();
}

string SKElementParser::ExtractOnMatch(string expected, SKToken **check)
{
    string returnvalue = "";
    if((*check)->next == NULL) throw ParseException("Next Token Missing.");
    if((*check)->next->Name() != expected) 
    {
        string s = "Unexpected Token: " + (*check)->next->Name() + " expected " + expected;
        throw ParseException(s);
    }

    *check = (*check)->next;
    returnvalue = (*check)->MatchedValue();
    return returnvalue;
}

float SKElementParser::ExtractNumber(SKToken **check)
{
    float returnvalue = 0;
    bool negative = false;
    if((*check)->next == NULL) throw ParseException{"ExtractNumber: expected number, found NULL."};
    if(PeekNextToken((*check)) == NegTok)
    {
        negative = true;
        *check = (*check)->next;
    }
    if((*check)->next->Name() == DigitTok)
    {
        returnvalue=stof((*check)->next->MatchedValue())*(negative?-1:1);
        *check = (*check)->next;
    }
    else
    {
        string s = "Expected DigitTok, got " + (*check)->next->Name();
        throw ParseException(s);
    }
    return returnvalue;
}

SKRValue *SKElementParser::ExtractRValue(SKToken **check, SKContext *context)
{
    SKRValue *returnvalue = NULL;
    bool negative = false;
    
    if((*check)->next == NULL) throw ParseException{"Expected NegTok, VarTok, NumTok or valid StateVarTok Value - got NULL."};
    if(PeekNextToken((*check)) == NegTok)
    {
        negative = true;
        *check = (*check)->next;
    }
    if((*check)->next->Name() == VarTok)
    {
        string varname = (*check)->next->MatchedValue();
        SKVariable *v = context->GetVariable(varname);
        if(v != NULL)
        {
            SKSingleRValue vv(varname, _model, negative);
            returnvalue = (SKRValue *)_model->AddValue((SKValue *)&vv);
            *check = (*check)->next;
        }
        else
        {
            string s = "Variable " + (*check)->next->MatchedValue() + " not found." ;
            throw ParseException{s};
        }
    }
    else if((*check)->next->Name() == KeyDownTok)
    {
        SKSingleRValue vv(to_string((int)KeyState::KeyDown), _model, false);
        returnvalue = (SKRValue *)_model->AddValue((SKValue *)&vv);
        *check = (*check)->next;
    }
    else if((*check)->next->Name() == KeyUpTok)
    {
        SKSingleRValue vv(to_string((int)KeyState::KeyUp), _model, false);
        returnvalue = (SKRValue *)_model->AddValue((SKValue *)&vv);
        *check = (*check)->next;
    }
    else if((*check)->next->Name() == MemberTok ||
            (*check)->next->Name() == CameraMemberTok ||
            (*check)->next->Name() == LightMemberTok ||
            (*check)->next->Name() == MouseMemberTok)
    {
        SKSingleRValue vv((*check)->next->MatchedValue(), _model, false);
        returnvalue = (SKRValue *)_model->AddValue((SKValue *)&vv);
        *check = (*check)->next;
    }
    else if((*check)->next->Name() == DigitTok)
    {
        SKSingleRValue temp((*check)->next->MatchedValue(), _model, negative);
        returnvalue = (SKRValue *)_model->AddValue((SKValue *)&temp);
        *check = (*check)->next;
    }
    else
    {
        string s = "Expected DigitTok, got " + (*check)->next->Name();
        throw ParseException(s);
    }
    return returnvalue;
}

SKRValue *SKElementParser::ExtractEquation(SKToken **check, SKContext *context) //TODO: NegTok DigitTok needs to be put together (Lexing issue)
{
    SKRValue *returnvalue = NULL;
    SKRValue *r1 = ExtractRValue(check, context);
    list<SKRValue *> v; 
    v.push_back(r1);
    bool done = false;

    while(!done)
    {
        if(IsOperableRValueToken(PeekNextToken(*check)))
            v.push_back(ExtractRValue(check, context));
        else if(IsOperationToken(PeekNextToken(*check)))
        {
            string match;
            string token = ExtractNextToken(check, false, match); //TODO: THIS MIGHT BE DIGIT PROBLEM (NEED TRUE)
            SKOperationRValue::SKStackOperation op = SKOperationRValue::SKStackOperation::StackPlus;
            if(token == PlusTok)
                op = SKOperationRValue::SKStackOperation::StackPlus;
            else if(token == NegTok)
                op = SKOperationRValue::SKStackOperation::StackMinus;
            else if(token == DivideTok)
                op = SKOperationRValue::SKStackOperation::StackDivide;
            else if(token == MultiplyTok)
                op = SKOperationRValue::SKStackOperation::StackMultiply;
            else if(token == ModulusTok)
                op = SKOperationRValue::SKStackOperation::StackModulus;
            else if(token == CosTok)
                op = SKOperationRValue::SKStackOperation::StackCos;
            else if(token == SinTok)
                op = SKOperationRValue::SKStackOperation::StackSin;
            else if(token == SignTok)
                op = SKOperationRValue::SKStackOperation::StackSign;
            else if(token == AbsTok)
                op = SKOperationRValue::SKStackOperation::StackAbs;
                  
            SKOperationRValue opVal(_model, op);
            v.push_back((SKRValue *)_model->AddValue((SKValue *)&opVal));
        }
        else if(PeekNextToken(*check) == EndMarkTok)
        { 
            done = true;
            ExtractOnMatch(EndMarkTok, check);
        }    
        else
            throw ParseException{"Invalid equation structure."};    
    }
    
    if(v.size()==1)
        returnvalue = r1;
    else
    {
        SKEquationRValue eqR(_model, v);
        returnvalue = (SKRValue *)_model->AddValue((SKRValue *)&eqR);
    }
    return returnvalue;
}

//TODO: Fix these - this breaks open closed principle
bool SKElementParser::IsOperableRValueToken(string value)
{
    bool returnvalue = false;
    if( value == VarTok ||
        value == MemberTok ||
        value == CameraMemberTok ||
        value == LightMemberTok ||
        value == DigitTok ||
        value == OpenTok)
        returnvalue = true;
    return returnvalue;
}

bool SKElementParser::IsOperationToken(string value)
{
    bool returnvalue = false;
    if(value == PlusTok ||
        value == NegTok ||
        value == MultiplyTok ||
        value == DivideTok ||
        value == ModulusTok ||
        value == SinTok ||
        value == CosTok ||
        value == SignTok ||
        value == AbsTok)
        returnvalue = true;
    return returnvalue;
}