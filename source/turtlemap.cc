#include "turtlemap.h"
#include "entity.h"
#include "projection.h"
#include "math.h"
#include <stack>
#include <algorithm>

#include <iostream>
using namespace std;

void SKTurtleMap::AddMove(SKMapMove *move)
{
    _moves.push_back(move);
}

void SKTurtleMap::BuildMap(float distance, float x, float y, float z)
{
    _tilesize = distance;
    float xAdjust = 0.0;
    float yAdjust = 0.0;
    float zAdjust = 0.0;
    _startX = x;
    _startY = y;
    _startZ = z;
    _positions.clear();
    _positions.push_back(SKMapLocation(Vector4{x, y, z, 1.0}, 0,0,0));
    stack<SKMapLocation> lastStack;
    SKMapLocation previous(_positions[_positions.size()-1]);
    lastStack.push(previous);
    for(auto a: _moves)
    {
        cout << a->Debug() << endl;
        if(a->GetDirection() == SKMapMove::Direction::Forward)
        {
            int amount = (int)a->GetAmount();
            for(int i = 0;i<amount;i++)
            {
                SKMapLocation p = lastStack.top(); 
                lastStack.pop();
                Vector4 direction(xAdjust, yAdjust, distance+zAdjust, 1);
                direction.Apply(_xTurn);
                direction.Apply(_yTurn);
                direction.Apply(_zTurn);
                Vector4 next = p._direction + direction;
                SKMapLocation n(next, _xRotation, _yRotation, _zRotation);
                _positions.push_back(n);
                lastStack.push(n);

                xAdjust = yAdjust = zAdjust = 0.0;
            }
        }
        else if(a->GetDirection() == SKMapMove::Direction::AdjustX)
            xAdjust = _tilesize * a->GetAmount();
        else if(a->GetDirection() == SKMapMove::Direction::AdjustY)
            yAdjust = _tilesize * a->GetAmount();
        else if(a->GetDirection() == SKMapMove::Direction::AdjustZ)
            zAdjust = _tilesize * a->GetAmount();
        else if(a->GetDirection() == SKMapMove::Direction::SetPoint)
            lastStack.push(lastStack.top());
        else if(a->GetDirection() == SKMapMove::Direction::ReturnToPoint)
            lastStack.pop();
        else if(a->GetDirection() == SKMapMove::Direction::TurnY)
            _yTurn.ApplyRotationY(a->GetAmount()/180*_pi);
        else if(a->GetDirection() == SKMapMove::Direction::TurnX)
            _xTurn.ApplyRotationX(a->GetAmount()/180*_pi);
        else if(a->GetDirection() == SKMapMove::Direction::TurnZ)
            _zTurn.ApplyRotationZ(a->GetAmount()/180*_pi);
        else if(a->GetDirection() == SKMapMove::Direction::RotateY)
            _yRotation = a->GetAmount();
        else if(a->GetDirection() == SKMapMove::Direction::RotateX)
            _xRotation = a->GetAmount();
        else if(a->GetDirection() == SKMapMove::Direction::RotateZ)
            _zRotation = a->GetAmount();
    }
}

vector<SKMapLocation> SKTurtleMap::GetMapLocations(SKProjection *projection, float distance)
{
    vector<SKMapLocation> returnvalue;
    for(auto a: _positions)
    {
        bool pointvisible = projection->PointVisible(a._direction);
        if(pointvisible)
        {
            float d = GetDistance(a._direction, projection->Eye());

            if(d <= distance)
            {
                a._lastDistance = d;
                returnvalue.push_back(a);
            }
        }
    }
    struct 
    {
        bool operator()(SKMapLocation a, SKMapLocation b)
        {
            return a._lastDistance<b._lastDistance;
        }
    } comparelocationdistance;
    std::sort(returnvalue.begin(), returnvalue.end(), comparelocationdistance);
    return returnvalue;
}

float SKTurtleMap::GetDistance(Vector4 point, Vector4 eye)
{
    float returnvalue =  sqrt(pow(point.x-eye.x, 2) +
                              pow(point.y-eye.y, 2) +
                              pow(point.z-eye.z, 2)); 

    return returnvalue;
    
}