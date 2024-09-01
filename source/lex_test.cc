#include "lex.h"
#include "parser.h"
#include "token.h"
#include "tokensym.h"
#include "regex.h"
#include "log.h"
#include <list>
#include <vector>
#include <string>
#include "exceptions.h"

using namespace std;

void test0()
{
    SKLexer l;
    l.AddToLexer(EqTok, "==");
    l.AddToLexer(AssignTok, "=");
    list<SKToken *> tok;
    l.Lex((char *)"==", &tok, false);
    if(tok.size() != 1) throw ParseException {"test 0: invalid size"};
    if(tok.front()->Name() != "EqTok") throw ParseException {"test 0: invalid name"};
    if(tok.front()->MatchedValue() != "==") throw ParseException {"test 0: invalid value"};
}

void test1()
{
    SKLexer l;
    l.AddToLexer(EqTok, "==");
    l.AddToLexer(AssignTok, "=");
    list<SKToken *> tok;
    l.Lex((char *)"=", &tok, false);
    if(tok.size() != 1) throw ParseException {"test 1: invalid size"};
    if(tok.front()->Name() != "AssignTok") throw ParseException {"test 1: invalid name"};
    if(tok.front()->MatchedValue() != "=") throw ParseException {"test 1: invalid value"};
}

void test2()
{
    SKLexer l;
    l.AddToLexer(ColorTok, "color");
    list<SKToken *> tok;
    l.Lex((char *)"color", &tok, false);
    if(tok.size() != 1) throw ParseException {"test 2: invalid size"};
    if(tok.front()->Name() != "ColorTok") throw ParseException {"test 2: invalid name"};
    if(tok.front()->MatchedValue() != "color") throw ParseException {"test 3: invalid value"};
}

void test10()
{
    SKLexer l;
    l.AddToLexer(ColorTok, "color");
    l.AddToLexer(VarTok, "\\w+");
    list<SKToken *> tok;
    l.Lex((char*)"colors", &tok, false);
    if(tok.size() != 1) throw ParseException {"test 10: invalid size"};
    if(tok.front()->Name() != "VarTok") throw ParseException {"test 10: invalid name"};
    if(tok.front()->MatchedValue() != "colors") throw ParseException {"test 10: invalid value"};
}


void test3()
{
    SKLexer lex;
    SKLog::SetLevel(SKLogType::Verbose);
    
    lex.AddToLexer(VarTok, "\\w+");
    lex.AddToLexer(DigitTok, "\\d+");

    lex.AddToLexer(StartTok, "(");
    lex.AddToLexer(EndTok, ")");
    lex.AddToLexer(AttachTok, "~");
    lex.AddToLexer(NegTok, "-");    

    list<SKToken *> tok;
    char x[] = "fred~rotate(10 10 10)";
    lex.Lex(x, &tok, true);
    if(tok.size() != 8) throw ParseException {"test 3: invalid size"};

    vector<SKToken *> tok2;
    for(auto it=tok.begin();it!=tok.end();++it)
        tok2.push_back((*it));
    
    if(tok2[0]->Name() != "VarTok") throw ParseException {"test 3: 0 name wrong."};
    if(tok2[1]->Name() != "AttachTok") throw ParseException {"test 3: 1 name wrong."};
    if(tok2[2]->Name() != "VarTok") throw ParseException {"test 3: 2 name wrong."};
    if(tok2[3]->Name() != "StartTok") throw ParseException {"test 3: 3 name wrong."};
    if(tok2[4]->Name() != "DigitTok") throw ParseException {"test 3: 4 name wrong."};
    if(tok2[5]->Name() != "DigitTok") throw ParseException {"test 3: 5 name wrong."};
    if(tok2[6]->Name() != "DigitTok") throw ParseException {"test 3: 6 name wrong."};
    if(tok2[7]->Name() != "EndTok") throw ParseException {"test 3: 7 name wrong."};
}

void test4()
{
    SKLexer lex;
    lex.AddToLexer(VarTok, "\\w+");
    list<SKToken *> tok;
    lex.Lex((char*)"r", &tok, false);
    if(tok.size() != 1) throw ParseException {"test4: invalid size"};
    if(tok.front()->Name() != "VarTok") throw ParseException {"test4: bad name"};
    if(tok.front()->MatchedValue() != "r") throw ParseException {"test4: bad match"};
}

void test5()
{
    SKLexer lex;
    lex.AddToLexer(MemberTok, "o.\\w+");
    list<SKToken *> tok;
    lex.Lex((char*)"o.rx", &tok, false);
    if(tok.size() != 1) throw ParseException {"test5: invalid size"};
    if(tok.front()->Name() != "MemberTok") throw ParseException {"test5: bad name"};
    if(tok.front()->MatchedValue() != "o.rx") throw ParseException {"test5: bad match"};
}

void test6()
{
    SKLexer lex;
    lex.AddToLexer(DigitTok, "\\d+.\\d+");
    list<SKToken *> tok;
    lex.Lex((char*)"1.0", &tok, false);
    if(tok.size() != 1) throw ParseException {"test6: invalid size"};
    if(tok.front()->Name() != "DigitTok") throw ParseException {"test6: bad name"};
    if(tok.front()->MatchedValue() != "1.0") throw ParseException {"test6: bad match"};
    if(stof(tok.front()->MatchedValue()) != 1.0) throw ParseException {"test6: bad conversion"};
}

int main() 
{
    test0();
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test10();
}
