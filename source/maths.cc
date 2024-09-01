#include "maths.h"
#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>

const float rounder = 100000.0;
const float troundr = 1000.0;
const float proundr = 100.0;

float myround(float x)
{
    float returnvalue = (int)(x * rounder + .5);
    returnvalue = (float)(returnvalue/rounder);
    return returnvalue;
}

float precisionround(float x)
{
    float returnvalue = ceil((int)(x * proundr + .5));
    returnvalue = (float)(returnvalue/proundr);
    return returnvalue;
}

float tightround(float x)
{
    float returnvalue = (int)(x * troundr + .5);
    returnvalue = (float)(returnvalue/troundr);
    return returnvalue;
}

int keyround(float x)
{
    int returnvalue = (int)(x * rounder + .5);
    return returnvalue;
}

string Vector4::DebugString()
{
    return string(to_string(x)+", "+to_string(y) + ", " + to_string(z)+ ", "+ to_string(w));
}

void Vector4::Scale(float s)
{
    x *= s;
    y *= s;
    z *= s;
}

Vector4& Vector4::operator=(const Vector4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

bool Vector4::operator==(const Vector4& other)
{
    bool returnvalue = false;
    if(tightround(other.x-x) == 0 &&
       tightround(other.y-y) == 0 &&
       tightround(other.z-z) == 0 &&
       tightround(other.w-w) == 0)
       returnvalue = true;
    return returnvalue;
}

void Vector4::Apply(const Matrix4x4& m)
{
    float x11 = m.rows[0][0]*x + m.rows[0][1]*y + m.rows[0][2]*z + m.rows[0][3]*w;
    float x21 = m.rows[1][0]*x + m.rows[1][1]*y + m.rows[1][2]*z + m.rows[1][3]*w;
    float x31 = m.rows[2][0]*x + m.rows[2][1]*y + m.rows[2][2]*z + m.rows[2][3]*w;
    x = myround(x11+m.rows[3][0]);
    y = myround(x21+m.rows[3][1]);
    z = myround(x31+m.rows[3][2]);
}

Vector4 Vector4::Minus(const Vector4& op)
{
    Vector4 result;
    result.x = myround(x-op.x);
    result.y = myround(y-op.y);
    result.z = myround(z-op.z);
    result.w = 1;
    return result;
}

Vector4 Vector4::Cross(const Vector4& op)
{
    Vector4 result;
    result.x = myround(y*op.z - z*op.y);
    result.y = myround(z*op.x - x*op.z);
    result.z = myround(x*op.y - y*op.x);
    result.w = 1;
    return result;
}

Vector4 Vector4::Normalize()
{
    Vector4 result;
    float magnitude = sqrt(x*x+y*y+z*z);
    if(magnitude == 0)
    {
        result.x = 0.0;
        result.y = 0.0;
        result.z = 0.0;
    }
    else
    {
        result.x = myround(x/magnitude);
        result.y = myround(y/magnitude);
        result.z = myround(z/magnitude);
    }
    result.w = 1;
    return result;
}

Vector4 Vector4::operator+(const Vector4& v)
{
    Vector4 returnvalue;
    returnvalue.x = x + v.x;
    returnvalue.y = y + v.y;
    returnvalue.z = z + v.z;
    returnvalue.w = 1.0;
    return returnvalue;
}

bool Vector4::operator!=(const Vector4 &v)
{
    bool returnvalue = false;
    if(myround(v.x) != myround(x) ||
       myround(v.y) != myround(y) ||
       myround(v.z) != myround(z))
       returnvalue = true;
    return returnvalue;
}

float Vector4::Dot(const Vector4& op)
{
    float returnvalue = myround(x*op.x + y*op.y + z*op.z);
    return returnvalue;
}

float Vector4::Distance(const Vector4& other)
{
    return sqrt(pow(x-other.x, 2) + 
                    pow(y-other.y, 2)+
                    pow(z-other.z, 2));
}

Matrix4x4::Matrix4x4()
{
    rows[0][0] = 1; rows[0][1] = 0; rows[0][2] = 0; rows[0][3] = 0;
    rows[1][0] = 0; rows[1][1] = 1; rows[1][2] = 0; rows[1][3] = 0;
    rows[2][0] = 0; rows[2][1] = 0; rows[2][2] = 1; rows[2][3] = 0;
    rows[3][0] = 0; rows[3][1] = 0; rows[3][2] = 0; rows[3][3] = 1;
}

int Matrix4x4::CandidateRowSwap(int row, int col)
{
    int bestrow = 0;
    int max = 0;
    for(int r=0;r<SIZE;r++)
    {
        if(r != row)
        {
            if(rows[r][col]>max)
            {
                max = abs(rows[r][col]);
                bestrow = r;
            }
        }
    }
    return bestrow;
}

void Matrix4x4::Inverse()
{
    //using reduced row elimination - adapted from scratchapixel [likely inspired by this - https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/matrix-inverse/matrix-inverse.html]
    Matrix4x4 returnvalue;
    //Swap Rows as needed
    for(int col=0;col<SIZE;col++)
    {
        for(int row=0;row<SIZE;row++)
        {
            if(col==row)
            {
                if(rows[row][col] == 0)
                {
                    int newrow = CandidateRowSwap(row, col);
                    SwapRows(row, newrow);
                    returnvalue.SwapRows(row, newrow);
                }
            }
        }
    }
    //Reduce to zero 
    for(int col=0;col<SIZE;col++)
    {
        for(int row=0;row<SIZE;row++)
        {
            if(col!=row)
            {
                float f = rows[row][col]/rows[col][col];
                if(f)
                {
                    for(int i=0;i<SIZE;i++)
                    {
                        rows[row][i] -= f * rows[col][i];
                        returnvalue.rows[row][i] -= f * returnvalue.rows[col][i];
                    }
                }
                rows[row][col] = 0;
            }
        }
    }
    for(int row=0;row<SIZE;row++)
    {
        for(int col=0;col<SIZE;col++)
        {
            returnvalue.rows[row][col] /= rows[row][row];
        }
    }
    for(int col=0;col<SIZE;col++)
    {
        for(int row=0;row<SIZE;row++)
        {
            rows[row][col] = returnvalue.rows[row][col];
        }
    }
}

void Matrix4x4::SwapRows(int a, int b)
{
    float temp[SIZE];
    for(int i=0;i<SIZE;i++)
    {
        temp[i] = rows[a][i];
        rows[a][i] = rows[b][i];
        rows[b][i] = temp[i];
    }
}

Matrix4x4::Matrix4x4(const Matrix4x4 &o)
{
    rows[0][0] = o.rows[0][0]; rows[0][1] = o.rows[0][1]; rows[0][2] = o.rows[0][2]; rows[0][3] = o.rows[0][3];
    rows[1][0] = o.rows[1][0]; rows[1][1] = o.rows[1][1]; rows[1][2] = o.rows[1][2]; rows[1][3] = o.rows[1][3];
    rows[2][0] = o.rows[2][0]; rows[2][1] = o.rows[2][1]; rows[2][2] = o.rows[2][2]; rows[2][3] = o.rows[2][3];
    rows[3][0] = o.rows[3][0]; rows[3][1] = o.rows[3][1]; rows[3][2] = o.rows[3][2]; rows[3][3] = o.rows[3][3];
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& o)
{
    rows[0][0] = o.rows[0][0]; rows[0][1] = o.rows[0][1]; rows[0][2] = o.rows[0][2]; rows[0][3] = o.rows[0][3];
    rows[1][0] = o.rows[1][0]; rows[1][1] = o.rows[1][1]; rows[1][2] = o.rows[1][2]; rows[1][3] = o.rows[1][3];
    rows[2][0] = o.rows[2][0]; rows[2][1] = o.rows[2][1]; rows[2][2] = o.rows[2][2]; rows[2][3] = o.rows[2][3];
    rows[3][0] = o.rows[3][0]; rows[3][1] = o.rows[3][1]; rows[3][2] = o.rows[3][2]; rows[3][3] = o.rows[3][3];
    return *this;
}

void Matrix4x4::Apply(const Matrix4x4 m)
{
    float x11 = m.rows[0][0]*rows[0][0] + m.rows[0][1]*rows[1][0] + m.rows[0][2]*rows[2][0] + m.rows[0][3]*rows[3][0];
    float x12 = m.rows[0][0]*rows[0][1] + m.rows[0][1]*rows[1][1] + m.rows[0][2]*rows[2][1] + m.rows[0][3]*rows[3][3];
    float x13 = m.rows[0][0]*rows[0][2] + m.rows[0][1]*rows[1][2] + m.rows[0][2]*rows[2][2] + m.rows[0][3]*rows[3][2];
    float x14 = m.rows[0][0]*rows[0][3] + m.rows[0][1]*rows[1][3] + m.rows[0][2]*rows[2][3] + m.rows[0][3]*rows[3][3];
    
    float x21 = m.rows[1][0]*rows[0][0] + m.rows[1][1]*rows[1][0] + m.rows[1][2]*rows[2][0] + m.rows[1][3]*rows[3][0];
    float x22 = m.rows[1][0]*rows[0][1] + m.rows[1][1]*rows[1][1] + m.rows[1][2]*rows[2][1] + m.rows[1][3]*rows[3][3];
    float x23 = m.rows[1][0]*rows[0][2] + m.rows[1][1]*rows[1][2] + m.rows[1][2]*rows[2][2] + m.rows[1][3]*rows[3][2];
    float x24 = m.rows[1][0]*rows[0][3] + m.rows[1][1]*rows[1][3] + m.rows[1][2]*rows[2][3] + m.rows[1][3]*rows[3][3];

    float x31 = m.rows[2][0]*rows[0][0] + m.rows[2][1]*rows[1][0] + m.rows[2][2]*rows[2][0] + m.rows[2][3]*rows[3][0];
    float x32 = m.rows[2][0]*rows[0][1] + m.rows[2][1]*rows[1][1] + m.rows[2][2]*rows[2][1] + m.rows[2][3]*rows[3][3];
    float x33 = m.rows[2][0]*rows[0][2] + m.rows[2][1]*rows[1][2] + m.rows[2][2]*rows[2][2] + m.rows[2][3]*rows[3][2];
    float x34 = m.rows[2][0]*rows[0][3] + m.rows[2][1]*rows[1][3] + m.rows[2][2]*rows[2][3] + m.rows[2][3]*rows[3][3];

    float x41 = m.rows[3][0]*rows[0][0] + m.rows[3][1]*rows[1][0] + m.rows[3][2]*rows[2][0] + m.rows[3][3]*rows[3][0];
    float x42 = m.rows[3][0]*rows[0][1] + m.rows[3][1]*rows[1][1] + m.rows[3][2]*rows[2][1] + m.rows[3][3]*rows[3][3];
    float x43 = m.rows[3][0]*rows[0][2] + m.rows[3][1]*rows[1][2] + m.rows[3][2]*rows[2][2] + m.rows[3][3]*rows[3][2];
    float x44 = m.rows[3][0]*rows[0][3] + m.rows[3][1]*rows[1][3] + m.rows[3][2]*rows[2][3] + m.rows[3][3]*rows[3][3];

    rows[0][0] = myround(x11); rows[0][1] = myround(x12); rows[0][2] = myround(x13); rows[0][3] = myround(x14);
    rows[1][0] = myround(x21); rows[1][1] = myround(x22); rows[1][2] = myround(x23); rows[1][3] = myround(x24);
    rows[2][0] = myround(x31); rows[2][1] = myround(x32); rows[2][2] = myround(x33); rows[2][3] = myround(x34);
    rows[3][0] = myround(x41); rows[3][1] = myround(x42); rows[3][2] = myround(x43); rows[3][3] = myround(x44);
}

Lerper::Lerper(Vector4 *lerpee, Vector4 target, int time)
{
    _time = time;
    _at = 0;
    _target = Vector4(target.x, target.y, target.z, 1);
    _lerpee = lerpee;
}

bool Lerper::Update()
{
    if(Done()) return false;
    _at += myround(1/_time);
    _lerpee->x = myround(_lerpee->x * (1-_at) + _target.x * _at);
    _lerpee->y = myround(_lerpee->y * (1-_at) + _target.y * _at);
    _lerpee->z = myround(_lerpee->z * (1-_at) + _target.z * _at);
    return true;
}

bool Lerper::Done()
{
    bool returnvalue = false;
    returnvalue = (_at == _time ||
    (myround(_lerpee->x) == myround(_target.x) &&
    myround(_lerpee->y) == myround(_target.y) &&
    myround(_lerpee->z) == myround(_target.z)));
    return returnvalue;
}
