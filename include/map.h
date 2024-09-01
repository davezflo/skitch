#ifndef __MAP__
#define __MAP__
#include <string>
#include <vector>
#include "maths.h"

using namespace std;

class SKProjection;
class SKMapLocation;
class SKMap
{
    private:
        string _name;
    public:
        SKMap(string name) {_name = name;}
        virtual ~SKMap() {}
        string GetName() {return _name;}
        virtual SKMap *AllocCopy() = 0;
        virtual void BuildMap(float distance, float x, float y, float z) = 0;
        virtual vector<SKMapLocation> GetMapLocations(SKProjection *projection, float distance) = 0;
};

class SKMapLocation
{
    public:
        SKMapLocation(Vector4 direction, float xrotate, float yrotate, float zrotate) 
        {
            _direction = Vector4(direction);
             _xRotate = xrotate; _yRotate = yrotate; _zRotate = zrotate;
        }
        string GetHash();
        Vector4 _direction;
        float _xRotate;
        float _yRotate;
        float _zRotate;
        float _lastDistance;
        string _hash;
};

#endif 