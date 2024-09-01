#ifndef __INFINIMAP__
#define __INFINIMAP__
#include "map.h"
#include <vector>
#include <list>
#include <map>

using namespace std;

class SKInfiniMapHold
{
    float _value;
    bool _hold;
    public:
        SKInfiniMapHold() {_hold = false; _value = 0.0;}
        void Initialize(bool hold, float value) {_value = value; _hold = hold;}
        bool GetHold() {return _hold;}
        float GetValue() {return _value;}      
        void operator=(const SKInfiniMapHold a)
        {
            _value = a._value;
            _hold = a._hold;
        }
};

class SKProjection;
class SKInfiniMap : SKMap
{
    private:
        float _x;
        float _y;
        float _z;
        float _tilesize;
        SKInfiniMapHold _xHold;
        SKInfiniMapHold _yHold;
        SKInfiniMapHold _zHold;
        SKProjection *_projection;
        float _distance;

    public:
        SKInfiniMap(string name, SKInfiniMapHold x, SKInfiniMapHold y, SKInfiniMapHold z) : SKMap(name)
        {
            _xHold = x;
            _yHold = y;
            _zHold = z;
            _x = _y = _z = 0.0;
            _distance = 0.0;
            _projection = NULL;
        }
        SKMap* AllocCopy() override
        {
            SKInfiniMap *returnvalue = new SKInfiniMap(GetName(), _xHold, _yHold, _zHold);
            return returnvalue;
        }
        void BuildMap(float distance, float x, float y, float z) override;
        vector<SKMapLocation> GetMapLocations(SKProjection *projection, float distance) override;

    private:
        void AddLocation(float x, float y, float z, list<string>& keys, vector<SKMapLocation>& locations);
        bool Inside(float a, float b, float c);

};



#endif