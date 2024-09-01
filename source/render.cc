#include "render.h"
#include "model.h"
#include "face.h"
#include "point.h"
#include "projection.h"
#include "log.h"
#include "light.h"
#include "context.h"
#include "numvar.h"
#include <algorithm>
#include <vector>

#define TICK 20
#define SQUARE 768
#define WINDOWWIDTH 1024
#define WINDOWHEIGHT 768
int WIDTH = 0;
int HEIGHT = 0;
#define GRID "GRID"
#define GRIDLINES "GRIDLINES"
#define ZPLANE "ZPLANE"

void SKRender::Initialize(SKModel *model)
{
    _model = model;
}

inline float GetCandidate(float y, float intercept, float slope, float x)
{
    float inf = std::numeric_limits<float>::infinity();
    float returnvalue = ((y-intercept)/slope);
    if(slope==inf||slope==-inf) returnvalue = x;
    if(slope == 0) returnvalue = inf;
    return returnvalue;
}

inline float GetAdjustedMin(float a, float b, float c, float adjustment)
{
    float returnvalue = a;
    if(b < returnvalue) {returnvalue = b; }
    if(c < returnvalue) {returnvalue = c; }
    returnvalue += adjustment;
    return returnvalue;
}

inline float GetAdjustedMax(float a, float b, float c, float adjustment)
{
    float returnvalue = c;
    if(a > returnvalue) {returnvalue = a; }
    if(b > returnvalue) {returnvalue = b; }
    returnvalue += adjustment;
    return returnvalue;
}

inline bool TestY(float y, float ay, float by)
{
    bool returnvalue = false;
    if(ay < by)
    {
        if(y > ay && y < by)
            returnvalue = true;
    }
    else if(ay > by)
    {
        if(y > by && y < ay)
            returnvalue = true;
    }
    return returnvalue;
}

void SKRender::FillTriangle(SDL_Renderer *renderer, 
                        float ax, float ay,
                        float bx, float by,
                        float cx, float cy, 
                        SKFace *face)
{
    if(face->GetFillColor()==NULL) 
        return;

   // SKLog::LogTime("", SKLog::Debug);
    
    float left = GetAdjustedMin(ax, bx, cx, -1);
    float right = GetAdjustedMax(ax, bx, cx, 1);
    float top = GetAdjustedMin(ay, by, cy, -1);
    float bottom = GetAdjustedMax(ay, by, cy, 1);

    float m1 = ((by-ay)/(bx-ax));
    float b1 = (by-m1*bx);
    float m2 = ((cy-by)/(cx-bx));
    float b2 = (by-m2*bx);
    float m3 = ((ay-cy)/(ax-cx));
    float b3 = (ay-m3*ax);

    if(right < 0 || left > WIDTH || top > HEIGHT || bottom < 0) return;

    struct 
    {
        bool operator()(float a, float b)
        {
            return a<b;
        }
    } comparex;

    SKColorVariable *f = face->GetFillColor();
    if(f == NULL) return;
    vector<SDL_FPoint> points;
    
    // if(face->IsRendered() && _pointCache.count(face))
    //      points = _pointCache[face];
    // else
    {
        bool dropout = false;
        for(float yscan = top;yscan<bottom;yscan+=1)
        {
            if(dropout) 
                break;
            if(yscan < 0) 
                continue;
            if(yscan > HEIGHT) 
                break;

            float l1 = GetCandidate(yscan, b1, m1, ax);
            float l2 = GetCandidate(yscan, b2, m2, bx);
            float l3 = GetCandidate(yscan, b3, m3, cx);

            float ascan = TestY(yscan, ay, cy);
            float bscan = TestY(yscan, cy, by);
            float cscan = TestY(yscan, by, ay);
            
            vector<float> xpoints;
            if(l1 != std::numeric_limits<float>::infinity() && cscan)
                xpoints.push_back(l1);
            if(l2 != std::numeric_limits<float>::infinity() && bscan)
                xpoints.push_back(l2);
            if(l3 != std::numeric_limits<float>::infinity() && ascan)
                xpoints.push_back(l3);
            std::sort(xpoints.begin(), xpoints.end(), comparex);
            if(xpoints.size()>=2)
            {
                points.push_back(SDL_FPoint {.x=xpoints[0], .y=yscan});
                points.push_back(SDL_FPoint {.x=xpoints[1], .y=yscan});
            }
        }
        if(!_pointCache.count(face))
            _pointCache.insert(pair<SKFace *, vector<SDL_FPoint>>(face, points));
        else
            _pointCache[face] = points;
        face->Rendered();
    }
    SDL_SetRenderDrawColor(renderer, 
                        f->GetRed(), f->GetGreen(), 
                        f->GetBlue(), f->GetAlpha());

    int count = points.size();
    if (count)
    {
        SDL_FPoint* pointset = &points[0];
        SDL_RenderDrawLinesF(renderer, pointset, count);
    }
  //  SKLog::LogTime("Fill Triangle", SKLog::Debug);
}

void SKRender::SortFaces(SKModel *model)
{
    allfaces.clear();
    vector<SKSurface> surfaces = model->GetSurfaces();
    list<SKShape *>::iterator it;
    
    for(auto s: surfaces)
    {   
        vector<SKFace *> faces = s.GetRenderedFaces();
        for(auto f: faces)
            allfaces.push_back(f);
    }
    struct 
    {
        bool operator()(SKFace *a, SKFace *b)
        {
            bool returnvalue = a->GetCenter().y<b->GetCenter().y;
            if(abs(a->GetCenter().y - b->GetCenter().y) < 0.1)
                returnvalue = a->Distance()>b->Distance();
            return returnvalue;
        }
    } compareFaceDistance;
    
    std::sort(allfaces.begin(), allfaces.end(), compareFaceDistance);
   
}

void SKRender::DrawPoint(SDL_Renderer *renderer, float x, float y, float radius)
{
    float pi = 2*acos(0.0);
    float r = radius;
    for(int i=0;i<360;i+=45)
    {
        float angle = i/180.0 * pi;
        float counterangle = ((i+180)%360)/180.0 * pi;
        float sx = r*cos(angle);
        float sy = r*sin(angle);
        float ex = r*cos(counterangle);
        float ey = r*sin(counterangle);
        SDL_RenderDrawLineF(renderer, x+sx, y+sy, x+ex, y+ey);
    }
}

float ZPlane(SKModel *model)
{
    float extent = 1.0;
    SKNumberVariable *v = (SKNumberVariable *)model->GetGlobalContext()->GetVariable(ZPLANE);
    if(v)
        extent = v->GetValue();
    else
    {
        extent = abs(model->GetProjection()->Eye().z);
        if(extent == 0) extent = abs(model->GetProjection()->Eye().y);
        if(extent == 0) extent = abs(model->GetProjection()->Eye().x);
    }
    return extent;
}

void SKRender::DrawGridLines(SDL_Renderer *renderer, SKModel *model, float linewidth)
{
    const int extentmultiple = 3;
    float extent = 1/ZPlane(model);
    float w = linewidth/ZPlane(model);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    _gridX.clear();
    _gridY.clear();
    vector<float> gridX;
    vector<float> gridY;
    for(float i=-w;i<extent*extentmultiple;i+=w)
    {
        Vector4 points = {i, i, 1, 0};
        _gridX.push_back(points.x);
        _gridY.push_back(points.y);
    }
    for(auto x: _gridX)
    {
        for(auto y: _gridY)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDrawPointF(renderer, x*WINDOWHEIGHT, y*WINDOWHEIGHT); 
        }
    }
}

void SKRender::DrawModel(SKModel *model, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_Texture *t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOWWIDTH, WINDOWHEIGHT);
    SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, t);
    SDL_SetRenderDrawColor(renderer, 0,0,0, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);

    SKNumberVariable *gl = (SKNumberVariable *)model->GetGlobalContext()->GetVariable(GRIDLINES);
    SKNumberVariable *grid = (SKNumberVariable *)model->GetGlobalContext()->GetVariable(GRID);
    if(gl && grid && gl->GetValue() == 1)
        DrawGridLines(renderer, model, grid->GetValue());
    
    if(model->WatchPoints())
    {
        vector<SKShape *> shapes = _model->GetShapes();
        for(auto s: shapes)
        {
            if(s->Interacts(INTERACT_POINTS))
            {
                vector<SKPoint *> points = s->GetPoints();
                SDL_SetRenderDrawColor(renderer, 255, 0xA5, 0, 255);
                for(auto p: points)
                {
                    p->SetExtent(SQUARE);
                    float ax = p->GetX(); float ay = p->GetY();
                    DrawPoint(renderer, ax, ay, 2.0);
                }
                points = s->GetSelectedPoints();
                SDL_SetRenderDrawColor(renderer, 0x32, 0xCD, 0x32, 255);
                for(auto p: points)
                {
                    p->SetExtent(SQUARE);
                    float ax = p->GetX(); float ay = p->GetY();
                    DrawPoint(renderer, ax, ay, 5.0);
                }
            }
        }
    }

    SKLog::LogTime("", SKLogType::Debug);
    SKPoint *p1, *p2, *p3;
    for(auto f=allfaces.begin();f!=allfaces.end();++f)
    {
        SKColorVariable *b = (*f)->GetBorderColor();
      
        (*f)->GetPoints(&p1, &p2, &p3, SQUARE);
        float ax = p1->GetX(); float ay = p1->GetY();
        float bx = p2->GetX(); float by = p2->GetY();
        float cx = p3->GetX(); float cy = p3->GetY();

        if(b != NULL)
        {
            SDL_SetRenderDrawColor(renderer, b->GetRed(), 
                                            b->GetGreen(),
                                            b->GetBlue(),
                                            b->GetAlpha());
            SDL_RenderDrawLineF(renderer, ax, ay, bx, by);
            SDL_RenderDrawLineF(renderer, bx, by, cx, cy);
            SDL_RenderDrawLineF(renderer, cx, cy, ax, ay);
        }
        FillTriangle(renderer,ax,ay,bx,by,cx,cy,(*f)); 
    }

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, t, NULL, NULL);
    SDL_DestroyTexture(t);
    
    _model->GetProjection()->Rendered();
    
    SKLog::LogTime("Draw Model", SKLogType::Debug);
}

Uint32 SKRender::UpdateModel(Uint32 interval, void *param)
{
    SKRender *render = (SKRender*)param;
    SKModel *model = render->_model;
    
    if(model->NeedsRender())
    {
        model->Update();
        SDL_Event event;
        SDL_UserEvent userevent;
        userevent.type = SDL_USEREVENT;
        userevent.code = 0;
        userevent.data1 = NULL;
        userevent.data2 = NULL;
        event.type = SDL_USEREVENT;
        event.user = userevent;
        SDL_PushEvent(&event);
    }
    
    return(TICK);
}

string Shifted(string key)
{
    string returnvalue = key;
    if(key == "1")
        returnvalue = "Exclamation";
    else if(key == "2")
        returnvalue = "At";
    else if(key == "3")
        returnvalue = "Hash";
    else if(key == "4")
        returnvalue = "Dollar";
    else if(key == "5")
        returnvalue = "Percent";
    else if(key == "6")
        returnvalue = "Caret";
    else if(key == "7")
        returnvalue = "Amp";
    else if(key == "8")
        returnvalue = "Asterisk";
    else if(key == "9")
        returnvalue = "OpenParen";
    else if(key == "0")
        returnvalue = "CloseParen";
    else if(key == "-")
        returnvalue = "Underscore";
    else if(key == "=")
        returnvalue = "Plus";
    else if(key == "[")
        returnvalue = "OpenCurly";
    else if(key == "]")
        returnvalue = "CloseCurly";
    else if(key == "\\")
        returnvalue = "Bar";
    else if(key == ";")
        returnvalue = "Colon";
    else if(key == ",")
        returnvalue = "Lt";
    else if(key == ".")
        returnvalue = "Gt";
    else if(key == "'")
        returnvalue = "Quote";
    else if(key == "/")
        returnvalue = "QuestionMark";
    return returnvalue;
}

string Examine(string key)
{
    string returnvalue = key;
    if(key == ";")
        returnvalue = "Semicolon";
    else if(key == "/")
        returnvalue = "ForwardSlash";
    else if(key == "\\")
        returnvalue = "BackSlash";
    else if(key == "=")
        returnvalue = "Equal";
    else if(key == "-")
        returnvalue = "Dash";
    else if(key == "[")
        returnvalue = "OpenSquare";
    else if(key == "]")
        returnvalue = "CloseSquare";
    else if(key == ",")
        returnvalue = "Comma";
    else if(key == ".")
        returnvalue = "Period";
    return returnvalue;
}

void SKRender::Run()
{
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) == 0)
    {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;
        SDL_TimerID timerid = SDL_AddTimer(20, SKRender::UpdateModel, this);
        window = SDL_CreateWindow("skitch", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                        WINDOWWIDTH, WINDOWHEIGHT, 0);
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        _model->SetRenderDimensions(WINDOWWIDTH, WINDOWHEIGHT);
        bool done = false;
        bool shift = false;
        while(!done) 
        {
            SDL_Event event;

            SDL_DisplayMode displayMode;
            SDL_GetCurrentDisplayMode(0, &displayMode);
            WIDTH = displayMode.w*1.2; HEIGHT = displayMode.h*1.2;
            
            SDL_WaitEvent(&event);
            if(event.type == SDL_QUIT)
                done = true;
            else if(event.type == SDL_USEREVENT)
            {
                SortFaces(_model);
                DrawModel(_model, renderer);             
                SDL_RenderPresent(renderer);
                _model->Rendered();
            }
            else if(event.type == SDL_KEYDOWN)
            {
                string keypressed = SDL_GetScancodeName(event.key.keysym.scancode);
                if(keypressed.find("Shift")!=string::npos) shift = true;
                if(shift) keypressed = Shifted(keypressed);
                else keypressed = Examine(keypressed);
                _model->FireKeyEvent(keypressed, KeyState::KeyDown);
                SKLog::Log(keypressed + " Down", SKLogType::Verbose);
            }
            else if(event.type == SDL_KEYUP)
            {
                string keypressed = SDL_GetScancodeName(event.key.keysym.scancode);
                if(keypressed.find("Shift")!=string::npos) shift = false;
                if(shift) keypressed = Shifted(keypressed);
                keypressed = Examine(keypressed);
                _model->FireKeyEvent(keypressed, KeyState::KeyUp);
                SKLog::Log(keypressed + " Up", SKLogType::Verbose);
            }
            int x, y;
            int fired = 0;
            if(SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
            {
                fired = 1;
                SKLog::Log("Left Mouse Down", SKLogType::Verbose);
            }
            else if(SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_RIGHT))
            {
                fired = 2;
                SKLog::Log("Right Mouse Down", SKLogType::Verbose);
            }

            float fx = x/(float)WINDOWHEIGHT;
            float fy = y/(float)WINDOWHEIGHT;
            Vector4 point = {fx, fy, 1, 0};
            Vector4 rawpoint = {fx, fy, 1, 0};
           
            float extent = ZPlane(_model);
            
            SKNumberVariable *_x = (SKNumberVariable *)_model->GetGlobalContext()->GetVariable(MOUSEX);
            SKNumberVariable *_y = (SKNumberVariable *)_model->GetGlobalContext()->GetVariable(MOUSEY);
            SKNumberVariable *rawx = (SKNumberVariable *)_model->GetGlobalContext()->GetVariable(RAW_MOUSEX);
            SKNumberVariable *rawy = (SKNumberVariable *)_model->GetGlobalContext()->GetVariable(RAW_MOUSEY);

            if(_x == NULL)
            {
                SKNumberVariable xx(MOUSEX);
                SKNumberVariable yy(MOUSEY);
                _x = (SKNumberVariable *)_model->GetGlobalContext()->AddVariable(&xx);
                _y = (SKNumberVariable *)_model->GetGlobalContext()->AddVariable(&yy);

                SKNumberVariable xx2(RAW_MOUSEX);
                SKNumberVariable yy2(RAW_MOUSEY);
                rawx = (SKNumberVariable *)_model->GetGlobalContext()->AddVariable(&xx2);
                rawy = (SKNumberVariable *)_model->GetGlobalContext()->AddVariable(&yy2);
            }
            //align to grid
            float distance = 100;
            float xCandidate = point.x;
            for(auto a: _gridX)
            {
                float d = abs(a-point.x);
                if(d<distance)
                {
                    xCandidate = a;
                    distance = d;
                }
            }
            distance = 100;
            float yCandidate = point.y;
            for(auto a: _gridY)
            {
                float d = abs(a-point.y);
                if(d<distance)
                {
                    yCandidate = a;
                    distance = d;
                }
            }

            point.x = xCandidate;
            point.y = yCandidate;

            Vector4 newpoint = _model->GetProjection()->ReverseCompute(point, extent);
            //Vector4 newraw = _model->GetProjection()->ReverseCompute(rawpoint, extent);
            _x->SetValue(newpoint.x);
            _y->SetValue(newpoint.y);
            rawx->SetValue(rawpoint.x);
            rawy->SetValue(rawpoint.y);

            SKLog::Log("Mouse Location: " + to_string(fx) + ", " + to_string(fy), SKLogType::Verbose);
            _model->FireMouseEvent(point.x, point.y, fired==1?MouseState::LeftButton:MouseState::RightButton);
    
        }
        SDL_RemoveTimer(timerid);
        if(renderer)
            SDL_DestroyRenderer(renderer);
        if(window)
            SDL_DestroyWindow(window);
    }

    SDL_Quit();
 }