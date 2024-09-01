#include <stdio.h>

#ifndef __STATES__
#define __STATES__

class State
{
    public:
        State *next;
        State *prev;

        State() { _valid = false; next = NULL; prev = NULL; }
        virtual ~State();


        virtual bool Match(char **value, State **n) {return false;}
        virtual bool ValidMatch() {return _valid;}
        void ResetState() {_valid = false;}

    protected:
        bool _valid;
};

class GroupState : public State
{
    private:
        State *collection;
        char *_values;
        int _length;

    public:
        GroupState(char* values, int length);
        bool Match(char **value, State **n) override;

    private:
        char MatchIn(char test);
};

class WordState : public State
{
    public:
        WordState();
        bool Match(char **value, State **n) override;
};

class DigitState : public State
{
    public:
        DigitState();
        bool Match(char **value, State **n) override;
};

class CharState : public State
{
    private:
        char _value;

    public:
        CharState(char value);
        bool Match(char** value, State **n) override;
};

class OneOrMore : public State
{
    public:
        OneOrMore();
        bool Match(char **value, State **n) override;

};

class InitialState : public State
{
    public:
        bool Match(char **value, State **n) override;
};

#endif
