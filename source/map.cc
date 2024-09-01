#include "map.h"
#include <sstream>

string SKMapLocation::GetHash()
{   
    if(_hash.size() > 0) return _hash;
    ostringstream db;
    db.precision(5); 
    db << "x" << _direction.x << "y" 
              << _direction.y << "z" 
              << _direction.z << "rx" 
              << _xRotate << "ry" 
              << _yRotate << "rz"
              << _zRotate;

    _hash = db.str();
    return _hash;
}