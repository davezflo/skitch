#include "action.h"
#include "actionsequence.h"
#include "entity.h"
#include "variable.h"
#include "numvar.h"
#include "model.h"
#include <string>
#include "context.h"
#include "exceptions.h"
#include "ticket.h"
#include "log.h"
#include <sstream>

using namespace std;
SKAction::SKAction(SKModel *model, string name, SKContext *initContext)
{
    _name = name;
    _model = model;
    _currentState = INIT;
    _attached = NULL;
    _context = new SKContext(initContext, _model->GetGlobalContext());
}

SKAction::~SKAction() 
{
    SKContext *s = _context;
    _context = NULL;
    delete(s);
}

void SKAction::AddActionSequence(SKActionSequence *sequence)
{
    _sequences.insert(pair<string, SKActionSequence *>(sequence->GetName(), sequence));
}

void SKAction::SetParameter(string value)
{
    _parameter = value;
}

void SKAction::SetTicket(string ticketname)
{
    _ticket = ticketname;
}

void SKAction::SetParentContext(SKContext *context)
{
    _context->SetParentContext(context);
}

SKAction *SKAction::AllocCopy()
{
    SKAction *returnvalue = new SKAction(_model, _name, _context);
    returnvalue->_currentState = _currentState;
    returnvalue->_attached = _attached;

    for(auto v: _variables)
        returnvalue->_variables.push_back(v);
  
    for(auto it =_sequences.begin();it!=_sequences.end();++it)
    {
        returnvalue->_sequences.insert((*it));
        (*it).second->PrepareForAction(returnvalue);
    }
    return returnvalue;
}

void SKAction::Update(SKEntity *s)
{
    string nextstate = "";
    Register(s);
    _model->PushContext(_context);
    
    if(_context->GetScratchString(NameState())== "")
    {
        SKClearTicket::Clear(_ticket, _model);
        UpdateCurrentState(INIT);
    }
    else
        UpdateCurrentState(_context->GetScratchString(NameState()));

    if(_sequences.count(_currentState))
    {
        SKActionSequence *sequence = _sequences[_currentState];
        nextstate = sequence->Update(_attached, _context, _ticket);
    }
    else if(_currentState != DEINIT) 
        throw StateException{string("Action: State not found: ") + _currentState};
    
    if(_currentState == DEINIT)
    {
        SKClearTicket::Clear(_ticket, _model);
        _attached->Detach(this);
    }
    else
    {
        UpdateCurrentState(nextstate);
        _context->SetScratchString(NameState(), _currentState);
    }
    _model->PopContext();
}

void SKAction::Deregister(SKEntity *s)
{
    _model->DeleteAction(this);
}

void SKAction::Register(SKEntity *s)
{
    _attached = s;
    _context->SetCurrentEntity(s);
    _context->SetScratchString(pa, _parameter);
}

void SKAction::Register(SKEntity *entity, vector<float> parameters)
{
    Register(entity);
    vector<float> p;
    for(auto it=parameters.begin();it!=parameters.end();++it)
        p.push_back((*it));

    unsigned int i = 0;
    for(auto a : _variables)
    {
        if(p.size()>i)
        {
            SKVariable *var = _context->GetVariable(a);
            if(var->GetType() == NUMBER_TYPE)
                ((SKNumberVariable *)var)->SetValue(p[i]);
        }
        i++;
    }
}

void SKAction::KeyEvent(string key, KeyState ks)
{
    string kskey = key + to_string((int)ks);
    SKLog::Log(kskey + " in action" + _name, SKLogType::Verbose);
    string state = _context->GetScratchString(NameState()) == ""?"init":_context->GetScratchString(NameState());
    if(_keymap.count(state))
    {
        map<string,string> transitions = _keymap[state];
        if(transitions.count(kskey))
        {
            state = transitions[kskey];
            _context->SetScratchString(NameState(), state);
        }
    }
}

void SKAction::MouseEvent(float x, float y, MouseState m)
{
    string mkey = to_string((int)m) + ":(" + to_string(x) + "," + to_string(y) + ")";
    string mkkey = to_string((int)m);
    SKLog::Log(mkey + " in action" + _name, SKLogType::Verbose);
    string state = _context->GetScratchString(NameState()) == ""?"init":_context->GetScratchString(NameState());
    if(_keymap.count(state))
    {
        map<string,string> transitions = _keymap[state];
        if(transitions.count(mkkey))
        {
            state = transitions[mkkey];
            _context->SetScratchString(NameState(), state);
        }
    }
}

void SKAction::UpdateCurrentState(string newstate)
{
    _currentState = newstate;
    UpdateStateHistory();
}

void SKAction::RegisterForKeyEvent(string key, string keystate, string instate, string tostate)
{
    KeyState ks = KeyState::KeyDown;
    if(keystate == STR_KEYUP) ks = KeyState::KeyUp;
    _model->RegisterKeyListener(key, ks, this);

    string kskey = key + keystate;
    map<string, string> keytostate;
    if(!_keymap.count(instate))
        _keymap.insert(pair<string, map<string, string>>(instate, keytostate));
    _keymap[instate].insert(pair<string,string>(kskey, tostate));
}

void SKAction::RegisterForMouseEvent(MouseState m, string instate, string tostate)
{
    _model->RegisterMouseListener(m, this);

    string mkkey = to_string((int)m);
    map<string, string> keytostate;
    if(!_keymap.count(instate))
        _keymap.insert(pair<string, map<string, string>>(instate, keytostate));
    _keymap[instate].insert(pair<string,string>(mkkey, tostate));
}

string SKAction::GetName()
{
    return _name;
}

string SKAction::DebugString()
{
    ostringstream db;
    db << "action(" << _name << "):";
    for(auto a: _stateHistory)
        db << a << "<-";
    
    const int MAXIMPORTANCE = 10;
    while(_stateHistory.size() > MAXIMPORTANCE)
        _stateHistory.pop_back();

    string returnvalue = db.str();
    return returnvalue; 
}

void SKAction::UpdateStateHistory()
{
    string add = _currentState;
    if(_stateHistory.size() > 0)
    {
        string last = _stateHistory[0];
        size_t x = last.find("(");
        if((x != string::npos && last.substr(0,x) == _currentState)
            || last == _currentState)
        {
            int n = 0;
            if(last != _currentState)
            {
                size_t y = last.find(")");
                string num = last.substr(x+1, y-x);
                n = atoi(num.c_str())+1;
            }
            _stateHistory.pop_front();
            add = _currentState + "(" + to_string(n) + ")";
        }
    }
    
    _stateHistory.push_front(add);
}