#include "projection.h"
#include <math.h>
#include "log.h"
#include "model.h"

SKProjection::SKProjection(SKModel *model)
{
    _model = model;
    _scalingFactor = 1;
    _eye.x = 0;
    _eye.y = 0;
    _eye.z = -5;
    _eye.w = 1;
    _adjustX = 0;
    _adjustY = 0;
    _orientation.x = _eye.x;
    _orientation.y = _eye.y;
    _orientation.z = _eye.z;
    _orientation.w = 1;
    _needsUpdate = 1;
    _pi = 2*acos(0.0);
    LookAt(0,0,0);
    UpdateAttention();
}

SKProjection::~SKProjection()
{
}

void SKProjection::LookAt(float x, float y, float z)
{
    _lookat = {x, y, z, 1};
    UpdateAttention();
}

void SKProjection::InitializeDimensions()
{
    _adjustX = _model->GetRenderDimensions().x>_model->GetRenderDimensions().y?
        _model->GetRenderDimensions().y/_model->GetRenderDimensions().x:
        0.5f;
    _adjustY = _model->GetRenderDimensions().y>_model->GetRenderDimensions().x?
        _model->GetRenderDimensions().x/_model->GetRenderDimensions().y:
        0.5f;
}

void SKProjection::UpdateAttention()
{
    Vector4 cameraPosition{_eye.x, _eye.y, _eye.z, 1};
    Vector4 cameraTarget = _lookat;
    Vector4 cameraUp(0, -1, 0, 1);
    Vector4 zaxis = cameraPosition.Minus(cameraTarget).Normalize();
    Vector4 xaxis = cameraUp.Cross(zaxis).Normalize();
    if(xaxis.x == xaxis.y && xaxis.y==xaxis.z && xaxis.x==0) xaxis.x = 1.0;
    Vector4 yaxis = zaxis.Cross(xaxis); 

    SKLog::Log(string("_direction: ")+to_string(_direction.x)+","+to_string(_direction.y)+","+to_string(_direction.z), SKLogType::Verbose);
    SKLog::Log(string("zaxis: ")+to_string(zaxis.x)+","+to_string(zaxis.y)+","+to_string(zaxis.z), SKLogType::Verbose);
    SKLog::Log(string("xaxis: ")+to_string(xaxis.x)+","+to_string(xaxis.y)+","+to_string(xaxis.z), SKLogType::Verbose);
    SKLog::Log(string("yaxis: ")+to_string(yaxis.x)+","+to_string(yaxis.y)+","+to_string(yaxis.z), SKLogType::Verbose);
    SKLog::Log(string("lookat:")+to_string(_lookat.x)+","+to_string(_lookat.y)+","+to_string(_lookat.z), SKLogType::Verbose);
    SKLog::Log(string("camera position:")+to_string(cameraPosition.x)+","+to_string(cameraPosition.y)+","+to_string(cameraPosition.z), SKLogType::Verbose);

    Matrix4x4 result;
    result.rows[0][0] = xaxis.x; result.rows[0][1] = xaxis.y; result.rows[0][2] = xaxis.z; result.rows[0][3] = 0;
    result.rows[1][0] = yaxis.x; result.rows[1][1] = yaxis.y; result.rows[1][2] = yaxis.z; result.rows[1][3] = 0;
    result.rows[2][0] = zaxis.x; result.rows[2][1] = zaxis.y; result.rows[2][2] = zaxis.z; result.rows[2][3] = 0;

    // result.a1 = xaxis.x; result.b1 = yaxis.x; result.c1 = zaxis.x; result.d1 = 0;
    // result.a2 = xaxis.y; result.b2 = yaxis.y; result.c2 = zaxis.y; result.d2 = 0;
    // result.a3 = xaxis.z; result.b3 = yaxis.z; result.c3 = zaxis.z; result.d3 = 0;

    result.rows[3][0] = -xaxis.Dot(cameraPosition);
    result.rows[3][1] = -yaxis.Dot(cameraPosition);
    result.rows[3][2] = -zaxis.Dot(cameraPosition);
    result.rows[3][3] = 1;
    // result.Apply(_xMatrix);
    // result.Apply(_yMatrix);
    // result.Apply(_zMatrix);

    double thetaX = atan2(result.rows[2][1], result.rows[2][2]);
    double thetaY = atan2(-result.rows[2][0], sqrt(pow(result.rows[2][1], 2) + pow(result.rows[2][2], 2)));
    double thetaZ = atan2(result.rows[1][0], result.rows[0][0]);

    double xx = thetaX * 180 / _pi;
    double yy = thetaY * 180 / _pi;
    double zz = thetaZ * 180 / _pi;

    SKLog::Log(string("derived camera angles: ")+to_string(xx)+","+to_string(yy)+","+to_string(zz), SKLogType::Verbose);
    SKLog::Log(string("derived translations: ")+to_string(result.rows[3][0])+","+to_string(result.rows[3][1])+","+to_string(result.rows[3][2]), SKLogType::Verbose);

    _attentionMatrix = result;

    _orientation.x = _attentionMatrix.rows[3][0];
    _orientation.y = _attentionMatrix.rows[3][1];
    _orientation.z = _attentionMatrix.rows[3][2];
    _orientation.w = 1;
}

void SKProjection::RotateX(float degrees)
{
    _xMatrix.ApplyRotationX(degrees/180*_pi);
}

void SKProjection::RotateY(float degrees)
{
    _yMatrix.ApplyRotationY(degrees/180*_pi);
}

void SKProjection::RotateZ(float degrees)
{
    _zMatrix.ApplyRotationZ(degrees/180*_pi);
}

void SKProjection::TranslateX(float distance)
{
    _eye.x = distance;
}

void SKProjection::TranslateY(float distance)
{
    _eye.y = distance;
}

void SKProjection::TranslateZ(float distance)
{
    _eye.z = distance;
}

void SKProjection::LookAtX(float location)
{
    _lookat.x = location;
}

void SKProjection::LookAtY(float location)
{
    _lookat.y = location;
}

void SKProjection::LookAtZ(float location)
{
    _lookat.z = location;
}

void SKProjection::Update()
{
    UpdateAttention();
}

Vector4 SKProjection::Compute(const Vector4& in)
{
    Vector4 out;
    Vector4 vec {in.x, in.y, in.z, 1};
    
    vec.Apply(_attentionMatrix);

    out.x = _scalingFactor*vec.x/abs(vec.z) + _adjustX;
    out.y = _scalingFactor*vec.y/abs(vec.z) + _adjustY;
    out.z = 1/vec.z;
    out.w = 1;
    return out;
}

Vector4 SKProjection::ReverseCompute(const Vector4& in, float zplane)
{
    Vector4 out;
    out.w = 1;
    out.y = in.y/_scalingFactor-_adjustY; out.y *= abs(zplane);
    out.x = in.x/_scalingFactor-_adjustX; out.x *= abs(zplane);

    out.y = (out.y);
    out.x = (out.x);

    return out;
}

bool SKProjection::PointVisible(Vector4 in)
{
    bool returnvalue = false;
    Vector4 diffin = in.Minus(_eye);
    if(_lookat.Minus(_eye).Dot(diffin)>=0)
        returnvalue = true;
    return returnvalue;
}