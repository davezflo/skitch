#include "parser.h"
#include <fstream>
#include "map.h"
#include "turtlemap.h"
#include "colorvar.h"
#include "strvar.h"
#include "numvar.h"
#include "tokensym.h"
#include "generate.h"
#include "rvalue.h"
#include "lvalue.h"
#include "model.h"
#include "point.h"
#include "face.h"
#include "log.h"
#include "shape.h"
#include "action.h"
#include "actionsequence.h"
#include "assignment.h"
#include "transition.h"
#include <string>
#include <exception>
#include <algorithm>
#include "exceptions.h"
#include "context.h"
#include "actionthread.h"
#include "lex.h"
#include "infinimap.h"
#include "elementparser.h"
#include "ticket.h"
#include "watch.h"
#include <iostream>
#include <sstream>

bool SKParser::Lex(string filename, list<SKToken *> *tokens)
{
    for(auto a: _parsedfiles)
    {
        if(a == filename) return true;
    }

    _parsedfiles.push_back(filename);
    bool returnvalue = false;
    ifstream ifs(filename);
    string s;
    if(ifs.good()) 
    {
        while(!ifs.eof())
            s.push_back(ifs.get());

        char *program = new char[s.length()+2];
        for(int i=0;i<(int)s.length();i++)
            program[i] = s[i];
        
        program[s.length()-1] = 0;
        program[s.length()+1] = 0;
        
        InitializeLexer();
        returnvalue = _lexer.Lex(program, tokens, false);
        delete[] program;
    }
    else
    {
        SKLog::Log("Invalid skitch file.", SKLogType::Basic);
        returnvalue = false;
    }
    
    return returnvalue;
}

void SKParser::Parse(SKModel *model, string filename)
{
    for(auto a: _parsers)
        a.second->Initialize(model, this);

    list<SKToken *> tokens;
    Lex(filename, &tokens);
    _model = model;
    SKToken *ptr = tokens.front();
    list<SKToken *>::iterator it;
    SKToken *at = NULL;
    for(it = tokens.begin();it!=tokens.end();++it)
    {
        (*it)->prev = at;
        if(at) 
        {
            at->next = (*it);
            at = (*it);
        }
        else
            at = (*it);
    }
    at->next = NULL;
    while(ptr)
    {
        ptr = _parsers[ptr->Name()]->Parse(ptr, _model->GetGlobalContext(), _parsers);
    }
}

void SKParser::InitializeLexer()
{
    if(_lexer.Initialized()) return;
    _lexer.AddToLexer(ColorTok, "color");
    _lexer.AddToLexer(StrTok, "str");
    _lexer.AddToLexer(NumTok, "num");
    _lexer.AddToLexer(ShapeTok, "shape");
    _lexer.AddToLexer(ActionTok, "action");
    _lexer.AddToLexer(MapTok, "map");
    _lexer.AddToLexer(PunchTok, "punch");
    _lexer.AddToLexer(WatchTok, "watch");
    _lexer.AddToLexer(InitStateTok, "init:");
    _lexer.AddToLexer(UseTok, "use");
    _lexer.AddToLexer(KeyDownTok, "keydown");
    _lexer.AddToLexer(KeyUpTok, "keyup");
    _lexer.AddToLexer(GenerateTok, "generator");
    _lexer.AddToLexer(StateVariableTok, "state");
    
    _lexer.AddToLexer(FractalTok, "fractal");
    _lexer.AddToLexer(RecolorTok, "recolor");

    _lexer.AddToLexer(AttachTok, "a:");
    _lexer.AddToLexer(OriginTok, "o:");
    _lexer.AddToLexer(CameraTok, "c:");
    _lexer.AddToLexer(PlayerTok, "p:");
    _lexer.AddToLexer(InteractTok, "i:");
    _lexer.AddToLexer(MapAttachTok, "m:");
    _lexer.AddToLexer(TransformTok, "t:");
    _lexer.AddToLexer(XTok, "x:");
    _lexer.AddToLexer(YTok, "y:");
    _lexer.AddToLexer(ZTok, "z:");

    _lexer.AddToLexer(TurtleForwardTok, "tf:");
    _lexer.AddToLexer(TurtleTurnYTok, "tty:");
    _lexer.AddToLexer(TurtleTurnXTok, "ttx:");
    _lexer.AddToLexer(TurtleTurnZTok, "ttz:");
    _lexer.AddToLexer(TurtleRotateYTok, "try:");
    _lexer.AddToLexer(TurtleRotateXTok, "trx:");
    _lexer.AddToLexer(TurtleRotateZTok, "trz:");
    _lexer.AddToLexer(TurtleSetPointTok, "ts:");
    _lexer.AddToLexer(TurtleReturnToPointTok, "tr:");
    _lexer.AddToLexer(TurtleAdjustXTok, "tax:");
    _lexer.AddToLexer(TurtleAdjustYTok, "tay:");
    _lexer.AddToLexer(TurtleAdjustZTok, "taz:");
    
    _lexer.AddToLexer(CosTok, "cos");
    _lexer.AddToLexer(SinTok, "sin");
    _lexer.AddToLexer(AbsTok, "abs");
    _lexer.AddToLexer(SignTok, "sign");

    _lexer.AddToLexer(FaceMemberTok, "[@].\\w+");
    _lexer.AddToLexer(PointMemberTok, "[p].\\w+");
    
    _lexer.AddToLexer(HexTok, "0x[abcdef0123456789]+");
    _lexer.AddToLexer(PointTok, "p\\d+");
    _lexer.AddToLexer(PointTok, "pn");
    _lexer.AddToLexer(MemberTok, "o.\\w+");
    _lexer.AddToLexer(WaitTok, ";\\w+");
    _lexer.AddToLexer(TicketTok, "'\\w+");
    _lexer.AddToLexer(CameraMemberTok, "c.\\w+");
    _lexer.AddToLexer(LightMemberTok, "l.\\w+");
    _lexer.AddToLexer(MouseMemberTok, "m.\\w+");
    _lexer.AddToLexer(KeyMemberTok, "k.\\w+");
    _lexer.AddToLexer(KeyMemberTok, "k.\\d+");
    _lexer.AddToLexer(StateDefTok, "\\w+:");
    _lexer.AddToLexer(DigitTok, "\\d+.\\d+");
    _lexer.AddToLexer(GenVarTok, "\\w+\\d+");
    _lexer.AddToLexer(VarTok, "\\w+");

    _lexer.AddToLexer(ThreadOpenTok, "~|");
    _lexer.AddToLexer(ThreadCloseTok, "|~");
    _lexer.AddToLexer(GteTok, ">=");
    _lexer.AddToLexer(LteTok, "<=");
    _lexer.AddToLexer(EqTok, "==");
    _lexer.AddToLexer(NeqTok, "!=");
    _lexer.AddToLexer(PlusAssignTok, "+=");
    _lexer.AddToLexer(MinusAssignTok, "-=");
    _lexer.AddToLexer(TimesAssignTok, "*=");
    _lexer.AddToLexer(DivideAssignTok, "/=");
    _lexer.AddToLexer(StateTransTok, "->");
    _lexer.AddToLexer(ConsumeTok, "<-");

    _lexer.AddToLexer(FaceTok, "@");
    _lexer.AddToLexer(ModulusTok, "%");
    _lexer.AddToLexer(OpenTok, "{");
    _lexer.AddToLexer(CloseTok, "}");
    _lexer.AddToLexer(AssignTok, "=");
    _lexer.AddToLexer(OtherTok, "_");
    _lexer.AddToLexer(StartTok, "(");
    _lexer.AddToLexer(EndTok, ")");
    _lexer.AddToLexer(NegTok, "-");
    _lexer.AddToLexer(PlusTok, "+");
    _lexer.AddToLexer(DivideTok, "/");
    _lexer.AddToLexer(MultiplyTok, "*");
    _lexer.AddToLexer(EndMarkTok, "!");
    _lexer.AddToLexer(CombineTok, "&");
    _lexer.AddToLexer(QueryTok, "?");
}
