#ifndef __TURTLEMAP__
#define __TURTLEMAP__
#include <list>
#include <string>
#include <vector>
#include <map>
#include "maths.h"
#include <math.h>
#include "map.h"
using namespace std;

class SKMapMove;
class SKMapLocation;
class SKEntity;
class SKProjection;
class SKTurtleMap : SKMap
{
    private:
        list<SKMapMove *> _moves;
        vector<SKMapLocation> _positions;
        map<string, float> _positionDistanceCache;
        Matrix4x4 _xTurn;
        Matrix4x4 _yTurn;
        Matrix4x4 _zTurn;
        float _xRotation;
        float _yRotation;
        float _zRotation;
        float _pi;
        float _tilesize;
        float _startX;
        float _startY;
        float _startZ;

    private:
        float GetDistance(Vector4 point, Vector4 eye);

    public:
        SKTurtleMap(string name) : SKMap(name)
        {
            _xRotation = 0; _yRotation = 0;
            _zRotation = 0; 
            _startX = 0;
            _startY = 0;
            _startZ = 0;
            _pi = (float)(2 * acos(0.0));
            _tilesize = 0.0;
        }
        virtual ~SKTurtleMap() {}
        
        SKMap *AllocCopy() override
        {
            SKTurtleMap *returnvalue = new SKTurtleMap(GetName());
            for(auto m: _moves)
                returnvalue->AddMove(m);
            return returnvalue;
        }
        void AddMove(SKMapMove *move);
        void BuildMap(float distance, float x, float y, float z) override;
        vector<SKMapLocation> GetMapLocations(SKProjection *projection, float distance) override;

};

class SKMapMove
{
    public:
        enum class Direction
        {
            Forward,
            TurnX,
            TurnY,
            TurnZ,
            RotateX,
            RotateY,
            RotateZ,
            SetPoint,
            ReturnToPoint,
            AdjustX,
            AdjustY,
            AdjustZ
        };
        Direction _direction;
        
        float _amount;

        SKMapMove(Direction direction, float amount)
        {
            _direction = direction;
            _amount = amount;
        }
        virtual ~SKMapMove() {}
        SKMapMove *AllocCopy()
        {
            SKMapMove *returnvalue = new SKMapMove(_direction, _amount);
            return returnvalue;
        }
        Direction GetDirection() {return _direction;}
        float GetAmount() {return _amount;}
        string Debug()
        {
            string returnvalue = "";
            switch(_direction)
            {
                case Direction::Forward:
                    returnvalue = "Forward";
                    break;
                case Direction::TurnX:
                    returnvalue = "TurnX";
                    break;
                case Direction::TurnY:
                    returnvalue = "TurnY";
                    break;
                case Direction::TurnZ:
                    returnvalue = "TurnZ";
                    break;
                case Direction::RotateX:
                    returnvalue = "RotateX";
                    break;
                case Direction::RotateY:
                    returnvalue = "RotateY";
                    break;
                case Direction::RotateZ:
                    returnvalue = "RotateZ";
                    break;
                case Direction::SetPoint:
                    returnvalue = "SetPoint";
                    break;
                case Direction::ReturnToPoint:
                    returnvalue = "ReturnToPoint";
                    break;
                case Direction::AdjustX:
                    returnvalue = "AdjustX";
                    break;
                case Direction::AdjustY:
                    returnvalue = "AdjustY";
                    break;
                case Direction::AdjustZ:
                    returnvalue = "AdjustZ";
                    break;
            }

            returnvalue += "(" + to_string(_amount) + ")";
            return returnvalue;
        }
};

#endif