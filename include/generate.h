
//
//generator enemy {
//  shape
//  actions
//  p 20
//  random min max or just a single number
//  direction 
//}

//p is player 
//o is origin
//c is camera
//a single number is distance
//3 numbers is more precise distance

//generator texture {
// shape
// actions
// c 0 0 2
// tile 
//}

//generator player {
// shape
// actions
// o 0 0 0
// 
//    
//}

//generate when the camera/player is close or at a given position
//generate at random intervals
//have it face a certain direction
//generate n of them
//create a pool to work from 

// generator genfloor {
//     floor
//     _
//     c: 0 0 2
//     x: -10 10
//     y: _
//     z: _
// }

#ifndef __GENERATOR__
#define __GENERATOR__

#include <string>
#include <list>
#include <vector>
#include <map>

using namespace::std;

enum RelativeTo
{
    Camera,
    Origin,
    Player,
    Map,
    None
};

class SKModel;
class SKProjection;
class SKMap;
class SKEntity;
class SKSimpleCacheEntry;
class SKGenerator
{
    class ActionSet
    {
        public:
            string action;
            vector<float> parameters;
    };
    private:
        SKModel *_model;
        string _entity;
        string _varname;
        list<string> _names;
        list<SKGenerator::ActionSet> _actions; 
        list<string> _interactions;
        vector<SKEntity *> _entities;
        vector<SKSimpleCacheEntry *> _cache;
        RelativeTo _location;
        float _x, _y, _z;
        float _limit;
        float _tilesize;
        float _distance;
        string _map;
        SKMap *_linkedMap;
        static int _nameIterator;
        int _namedIterator;
        bool _initialized;

    public:
        SKGenerator();
        virtual ~SKGenerator();
        SKGenerator *AllocCopy();
        void Initialize(SKModel *model, string entity, string varname, float limit);
        void SetMap(string map, float tilesize, float distance);
        void AddAction(string action, vector<float> parameters);
        void AddInteraction(string interaction);
        void SetLocation(RelativeTo relative, float x, float y, float z);
        void Update(SKProjection *projection);

    private:
        void InitializeMap();
        void ConstructCache();
        int GetEntry(string hash);
};

class SKSimpleCacheEntry
{
    public:
        string _hash;
        int _lastTouched;
        int _entry;
        SKSimpleCacheEntry(int entry)
        {
            _hash = "";
            _lastTouched = 0;
            _entry = entry;
        }
        SKSimpleCacheEntry(SKSimpleCacheEntry &s)
        {
            *this = s;
        }
        void operator=(SKSimpleCacheEntry &s)
        {
            _hash = s._hash;
            _lastTouched = s._lastTouched;
            _entry = s._entry;
        }
};

#endif