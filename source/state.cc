#include "state.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

State::~State() {}

CharState::CharState(char value) : State()
{
    _value = value;
}
bool CharState::Match(char **value, State **nextState) 
{
    bool returnvalue = false;
    _valid = false;
    *nextState = NULL;
    if(strlen(*value) == 0) return returnvalue;
    if(_value == (*value)[0])
    {
        _valid = true;
        *nextState = next;
        *value = *value + 1;
        returnvalue = true;
    }
    return returnvalue;
}

WordState::WordState() : State()
{
}

bool WordState::Match(char **value, State **nextState)
{
    bool returnvalue = false;
    _valid = false;
    *nextState = NULL;
    if(strlen(*value) == 0) return returnvalue;
    if(isalpha((*value)[0]))
    {
        _valid = true;
        *nextState = next;
        *value = *value + 1;
        returnvalue = true;
    }
    return returnvalue;
}

DigitState::DigitState() : State()
{
}


bool DigitState::Match(char **value, State **nextState)
{
    bool returnvalue = false;
    _valid = false;
    *nextState = NULL;
    if(strlen(*value) == 0) return returnvalue;
    if(isdigit((*value)[0]))
    {
        _valid = true;
        *nextState = next;
        *value = *value + 1;
        returnvalue = true;
    }
    return returnvalue;
}


GroupState::GroupState(char *values, int length) : State()
{
    collection = NULL;
    State *ptr = collection;
    char *cptr = values;
    int at = 0;
    while(at != length)
    {
        if(cptr[0] == '\\')
        {
            State *s = NULL;
            if(cptr[1] == 'd')
                s = new DigitState();
            else if(cptr[1] == 'w')
                s = new WordState();
            if(s)
            {
                cptr += 2;
                at += 2;
            }
            if(ptr)
            {
                ptr->next = s;
                s->prev = ptr;
                s->next = NULL;
            }
            else
            {
                collection = s;
                ptr = collection;
                s->next = NULL;
            }
        }
        else
        {
            State *s = new CharState(cptr[0]);
            if(ptr)
            {
                ptr->next = s;
                s->prev = ptr;
                s->next = NULL;
                ptr = s;
            }
            else
            {
                collection = s;
                ptr = collection;
                s->next = NULL;
            }
            cptr++;
            at++;
        }

    }
}

bool GroupState::Match(char **value, State **nextState)
{
    bool returnvalue = false;
    _valid = false;
    *nextState = NULL;
    if(strlen(*value) == 0) return returnvalue;
    State *ptr = collection;
    while(ptr)
    {
        if(ptr->Match(value, nextState))
        {
            _valid = true;
            *nextState = next;
            returnvalue = true;
            break;
        }
        else
            ptr = ptr->next;
    }
    return returnvalue;
}

OneOrMore::OneOrMore()
{
}

bool OneOrMore::Match(char **value, State **nextState) 
{
    bool returnvalue = false;
    _valid = true; //I wouldn't be here if the previous state hadn't already matched at least once.
    *nextState = NULL;
    if(prev->Match(value, nextState))
    {
        returnvalue = true;
        *nextState = this;
    }
    else if(!_valid)
        *nextState = NULL;
    else
        *nextState = next;
    return returnvalue;
}

bool InitialState::Match(char **value, State **nextState) {
    *nextState = next;
    return true;
}
