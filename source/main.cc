
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <fstream>
#include "lex.h"
#include "parser.h"
#include "model.h"
#include "elementparser.h"
#include "render.h"
#include "log.h"
#include <iostream>
#include <string>
#include <streambuf>
#include <list>
#include <limits>
#include <cmath>
#include <chrono>
#include <thread>

#include "parseaction.h"
#include "parsecolor.h"
#include "parsegenerate.h"
#include "parsemap.h"
#include "parsenumber.h"
#include "parseshape.h"
#include "parsestring.h"
#include "parseuse.h"
#include "parsewatch.h"
#include "parsestatevar.h"

using namespace std;

bool SetupModel(string filename, SKModel *model)
{
    SKParser p;
    SKParseColor c; p.AddParser(ColorTok, &c);
    SKParseAction a; p.AddParser(ActionTok, &a);
    SKParseGenerate g; p.AddParser(GenerateTok, &g);
    SKParseMap m; p.AddParser(MapTok, &m);
    SKParseNumber n; p.AddParser(NumTok, &n);
    SKParseString s; p.AddParser(StrTok, &s);
    SKParseStateVariable st; p.AddParser(StateVariableTok, &st);
    SKParseUse u; p.AddParser(UseTok, &u);
    SKParseWatch w; p.AddParser(WatchTok, &w);
    SKParseShape sh; p.AddParser(ShapeTok, &sh);
    p.Parse(model, filename);
    
    return true;
}

int main(int argc, char *argv[])
{
    SKLog::SetLevel(SKLogType::None);

    if(argc > 2)
    {
        if(strcmp(argv[2], "verbose")==0)
            SKLog::SetLevel(SKLogType::Verbose);
        else if(strcmp(argv[2], "basic")==0)
            SKLog::SetLevel(SKLogType::Basic);
        else if(strcmp(argv[2], "debug")==0)
            SKLog::SetLevel(SKLogType::Debug);
    }
    SKModel *model = new SKModel();
    SKRender *render = new SKRender();
    
    if (argc <= 1) return 0;
    if(SetupModel(argv[1], model))
    {
        render->Initialize(model);
        render->Run();
    }
    delete(model);
    delete(render);
    return 0;
}
