#include "infinimap.h"
#include "projection.h"
#include <string>
#include <list>
#include <algorithm>
#include <sstream>
#include <math.h>

using namespace std;

string CreateKey(float a, float b, float c)
{
    std::ostringstream key;
    key.precision(4); key << a << "." << b << "." << c;
    string returnvalue = key.str();
    return returnvalue;
}

bool HasKey(list<string> keys, float a, float b, float c)
{
    return count(keys.begin(), keys.end(), CreateKey(a, b, c))==1?true:false;
}

void SKInfiniMap::BuildMap(float distance, float x, float y, float z)
{
    _x = x; _y = y; _z = z;
    _tilesize = distance;
}

vector<SKMapLocation> SKInfiniMap::GetMapLocations(SKProjection *projection, float distance)
{
    _projection = projection;
    _distance = distance;
    vector<SKMapLocation> returnvalue;

    float a = 0;
    float b = 0;
    float c = 0;

    list<string> seen;
    
    if(_yHold.GetHold())
    {
        b = _yHold.GetValue() + _y;
        a = projection->Eye().x;
        c = projection->Eye().z;

        int tweak = int(int(a)%int(_tilesize));
        if(tweak < 0) tweak += _tilesize;
        a = int(a) - tweak;

        tweak = int(int(c)%int(_tilesize));
        if(tweak < 0) tweak += _tilesize;
        c = int(c) - tweak;

        AddLocation(a, b, c, seen, returnvalue);
    }

    return returnvalue;
}

bool SKInfiniMap::Inside(float a, float b, float c)
{
    float distance = sqrt(pow(a-_projection->Eye().x, 2)+
                          pow(b-_projection->Eye().y, 2)+
                          pow(c-_projection->Eye().z, 2));
    return distance<_distance?true:false;
}

void SKInfiniMap::AddLocation(float a, float b, float c, 
                            list<string>& keys, 
                            vector<SKMapLocation>& locations)
{
    bool pointvisible = _projection->PointVisible(Vector4(a, b, c, 1));
    if(!HasKey(keys, a, b, c) && Inside(a, b, c))
    {
        if(pointvisible)
            locations.push_back(SKMapLocation(Vector4(a, b, c, 1), 0, 0, 0));
        
        keys.push_back(CreateKey(a, b, c));
        
        AddLocation(a-_tilesize, b, c, keys, locations);
        AddLocation(a+_tilesize, b, c, keys, locations);
        AddLocation(a, b, c-_tilesize, keys, locations);
        AddLocation(a, b, c+_tilesize, keys, locations);
    }
}