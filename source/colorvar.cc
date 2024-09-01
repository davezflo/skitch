#include "colorvar.h"

SKColorVariable::SKColorVariable(string name)
{
    SetName(name);
    SetType("color");
}

SKColorVariable::SKColorVariable(uint32_t color)
{
    _red = (color >> 24) & 0xFF;
    _green = (color >> 16) & 0xFF;
    _blue = (color >> 8) & 0xFF;
    _alpha = color & 0xFF;
}
void SKColorVariable::Update(float intensity, const SKColorVariable &original)
{
    _red = intensity * original._red;
    _blue = intensity * original._blue;
    _green = intensity * original._green;
    _alpha = original._alpha;

}

string SKColorVariable::DebugString()
{
    return GetType() + "(" + GetName() + ") R: " + to_string(GetRed())
     + "G: " + to_string(GetGreen()) + "B: " + to_string(GetBlue()) + "\n";
}

SKVariable *SKColorVariable::AllocCopy()
{
    SKColorVariable *returnvalue = new SKColorVariable(GetName());
    returnvalue->_red = _red;
    returnvalue->_green = _green;
    returnvalue->_blue = _blue;
    returnvalue->_alpha = _alpha;
    return returnvalue;
}

void SKColorVariable::SetRed(int red)
{
    _red = red;
}

void SKColorVariable::SetGreen(int green)
{
    _green = green;
}

void SKColorVariable::SetBlue(int blue)
{
    _blue = blue;
}

void SKColorVariable::SetAlpha(int alpha)
{
    _alpha = alpha;
}
        
int SKColorVariable::GetRed()
{
    return _red;
}
int SKColorVariable::GetGreen()
{
    return _green;
}

int SKColorVariable::GetBlue()
{
    return _blue;
}
int SKColorVariable::GetAlpha()
{
    return _alpha;
}
