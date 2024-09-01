#include <iostream>
#include "regex.h"

using namespace std;
Regex::Regex(const char *pattern)
{
    stateChain = new InitialState();
    SetPattern(pattern);
}

Regex::Regex()
{
    stateChain = new InitialState();
}

Regex::~Regex()
{
    State *ptr = stateChain;
    while(ptr)
    {
        State *x = ptr;
        ptr = ptr->next;
        delete(x);
    }
}

void Regex::SetPattern(const char *pattern)
{
    State *ptr = stateChain;
    char *cptr = (char *)pattern;

    while(cptr[0] != '\0')
    {
        if(cptr[0] == '+' && ptr != stateChain) //exception for + at front
        {
            cptr++;
            State *s = new OneOrMore();
            ptr->next = s;
            s->prev = ptr;
            s->next = NULL;
        }
        else if(cptr[0] == '[')
        {
            char *nextptr = cptr+1;
            int count = 0;
            while(nextptr[0] != ']')
            {
                nextptr++;
                count++;
            }
            State *s = new GroupState(cptr+1, count);
            cptr = nextptr+1;
            ptr->next = s;
            s->prev = ptr;
            s->next = NULL;
        }
        else if(cptr[0] == '\\')
        {
            State *s = NULL;
            if(cptr[1] == 'd')
                s = new DigitState();
            else if(cptr[1] == 'w')
                s = new WordState();
            if(s)
            {
                cptr += 2;
                ptr->next = s;
                s->prev = ptr;
                s->next = NULL;
            }
        }
        else
        {
            State *s = new CharState(cptr[0]);
            ptr->next = s;
            s->prev = ptr;
            s->next = NULL;
            cptr++;
        }
        ptr = ptr->next;
    }
}

//Possible scenarios
//I run out of letters before all the states process.
//I run out of states before all the letters have been processed.
//A state fails somewhere along the way

string Regex::Match(char *match)
{
    //all states in the stateChain must be successful
    //if any fail, the entire thing fails

    State *s = stateChain->next;
    while(s) 
    {
        s->ResetState();
        s = s->next;
    }

    matchValue = "";
    s = stateChain->next;
    char *at = match;
    int index = 0;
    while(s)
    {
        char a = at[0];
        State *tempState = s;
        bool success = s->Match(&at, &s);
        if(success)
        {
            matchValue.push_back(a);
            index++;
        }
        else if(s==NULL) //Indicates we are done
        {
            //but was the last thing we captured valid?
            if(!tempState->ValidMatch())
                matchValue = "";
            break;
        }
        
    }
    return matchValue;
}

