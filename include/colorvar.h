#include "variable.h"

#ifndef __COLORVARIABLE__
#define __COLORVARIABLE__

class SKColorVariable : public SKVariable 
{
    private:
        int _red;
        int _green;
        int _blue;
        int _alpha;

    public:
        SKColorVariable(uint32_t color);
        SKColorVariable() {_red = 0;_blue=0;_green=0;_alpha=0xFFFF;}
        SKColorVariable(string name);
        SKVariable *AllocCopy() override;

        void Update(float intensity, const SKColorVariable& original);
        void SetRed(int red);
        void SetGreen(int green);
        void SetBlue(int blue);
        void SetAlpha(int alpha);
        
        int GetRed();
        int GetGreen();
        int GetBlue();
        int GetAlpha();
        string DebugString() override;
};
#endif
