#include <SDL2/SDL.h>
#include <fstream>
#include "lex.h"
#include "parser.h"

#include <iostream>
#include <string>
#include <streambuf>
#include <list>
#include <limits>
#include <cmath>
#include <vector>
#include <map>
#include <thread>
#include "light.h"

using namespace std;

class SKFace;
class SKModel;
class SKRenderInstruction;
class SKRender
{
    private:
        SKModel *_model;
        map<SKFace *, vector<SDL_FPoint>> _pointCache;
        vector<SKFace *> allfaces;
        vector<float> _gridX;
        vector<float> _gridY;
        
    public:
        SKRender()
        {
            _model = NULL;
        }
        void Initialize(SKModel *_model);
        void Run();

    private:
        void FillTriangle(SDL_Renderer *renderer, 
                        float ax, float ay,
                        float bx, float by,
                        float cx, float cy, 
                        SKFace *face);
        void DrawPoint(SDL_Renderer *renderer, float x, float y, float radius);
        void DrawGridLines(SDL_Renderer *renderer, SKModel *model, float linewidth);
        static Uint32 UpdateModel(Uint32 interval, void *param);
        void DrawModel(SKModel *model, SDL_Renderer *renderer);
        void SortFaces(SKModel *model);
};