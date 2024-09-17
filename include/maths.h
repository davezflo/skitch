#include <limits>
#include <cmath>
#include "update.h"
#include <string>
using namespace std;
#ifndef __MATHS__
#define __MATHS__

#define SIZE 4

const float SKM_PI = (float)(2 * acos(0));

float myround(float);
float precisionround(float);
int keyround(float);

class Matrix4x4
{

private:
    void SwapRows(int a, int b);
    int CandidateRowSwap(int row, int col);


public:
    float rows[SIZE][SIZE];
    float scale = 1.0;
   
    float xTheta = numeric_limits<float>::quiet_NaN();
    float yTheta = numeric_limits<float>::quiet_NaN();
    float zTheta = numeric_limits<float>::quiet_NaN();

    Matrix4x4();
    Matrix4x4(const Matrix4x4 &o);
    Matrix4x4& operator=(const Matrix4x4& o);

    void Inverse();

    void ApplyScale(float value)
    {
        scale = value;
    }
    
    void ApplyRotationX(float theta)
    {
        if(xTheta != theta)
        {
            xTheta = theta;
            float c = cos(theta)*scale;
            float s = sin(theta)*scale;
            rows[0][0] = scale; rows[0][1] = 0; rows[0][2] = 0; rows[0][3] = 0;
            rows[1][0] = 0; rows[1][1] = c; rows[1][2] = s; rows[1][3] = 0;
            rows[2][0] = 0; rows[2][1] = -s; rows[2][2] = c; rows[2][3] = 0;
        }
    }

    void ApplyRotationY(float theta)
    {
        if(yTheta != theta)
        {
            yTheta = theta;
            float c = cos(theta)*scale;
            float s = sin(theta)*scale;
            rows[0][0] = c; rows[0][1] = 0; rows[0][2] = -s; rows[0][3] = 0;
            rows[1][0] = 0; rows[1][1] = scale; rows[1][2] = 0; rows[1][3] = 0;
            rows[2][0] = s; rows[2][1] = 0; rows[2][2] = c; rows[2][3] = 0;
        }
    }

    void ApplyRotationZ(float theta)
    {
        if(zTheta != theta)
        {
            zTheta = theta;
            float c = cos(theta)*scale;
            float s = sin(theta)*scale;
            rows[0][0] = c; rows[0][1] = s; rows[0][2] = 0; rows[0][3] = 0;
            rows[1][0] = -s; rows[1][1] = c; rows[1][2] = 0; rows[1][3] = 0;
            rows[2][0] = 0; rows[2][1] = 0; rows[2][2] = scale; rows[2][3] = 0;
        }
    }

    void ApplyTranslationX(float x) 
    {
        rows[3][0] = x;
    }

    void ApplyTranslationY(float y)
    {
        rows[3][1] = y;
    }

    void ApplyTranslationZ(float z)
    {
        rows[3][2] = z;
    }

    void Apply(const Matrix4x4 m);

    
};

class Vector4
{
    public:
        float x;
        float y;
        float z;
        float w;
        Vector4() { x = 0; y = 0; z = 0; w = 1; }
        Vector4(float x, float y, float z, float w) 
        {this->x = x; this->y = y; this->z = z; this->w = w;}
        Vector4(const Vector4& v) {x = v.x, y=v.y, z= v.z, w=v.w;}
        bool operator==(const Vector4& other);
        Vector4& operator=(const Vector4& v);
        Vector4 operator+(const Vector4& v);    
        bool operator!=(const Vector4& v);
        Vector4 Cross(const Vector4& otherv);
        Vector4 Minus(const Vector4& otherv);
        Vector4 Normalize();
        float Dot(const Vector4& otherv);
        void Apply(const Matrix4x4& m);
        void Scale(float s);
        float Distance(const Vector4& otherv);
        string DebugString();
        const bool XNormal() const {if(abs(x)>abs(y) && abs(x)>abs(z)) return true; else return false;}
        const bool YNormal() const {if(abs(y)>abs(x) && abs(y)>abs(z)) return true; else return false;}
        const bool ZNormal() const {if(abs(z)>abs(y) && abs(z)>abs(x)) return true; else return false;}

        
};

class Lerper : IUpdate
{
    Vector4 *_lerpee;
    Vector4 _target;
    float _time;
    float _at;
    public:
        Lerper(Vector4 *lerpee, Vector4 target, int time);
        bool Update();
    private:
        bool Done();

};

#endif

