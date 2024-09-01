#ifndef __WATCH__
#define __WATCH__
#include <string>
#include <list>

#define NOTAPPLICABLE -1
#define WATCH_CAMERA "camera"
#define WATCH_LIGHT "light"
#define WATCH_FPS "fps"
#define WATCH_POINTS "points"
#define WATCH_MOUSE "mouse"

using namespace std;
class SKModel;
class SKWatchItem
{
    private:
        string _entity;
        int _face;
        int _point;

    public:
        SKWatchItem(const SKWatchItem &from)
        {
            _entity = from._entity;
            _face = from._face;
            _point = from._point;
        }
        SKWatchItem(string entity, int face, int point)
        {
            _entity = entity;
            _face = face;
            _point = point;
        }
        string Update(SKModel *model);
        string GetEntity() {return _entity;}
};


class SKWatch
{
    private:
        list<SKWatchItem> _watches;
        int _lastSize;

    public:
        SKWatch() {_lastSize = 0;}
        virtual ~SKWatch() {};

        bool WatchLight();
        bool WatchPoints();

        SKWatch *AllocCopy()
        {
            SKWatch *returnvalue = new SKWatch();
            for(auto a: _watches)
            {
                returnvalue->_watches.push_back(SKWatchItem(a));
            }
            return returnvalue;
        }

        void WatchVariable(string variable)
        {
            _watches.push_back(SKWatchItem(variable, NOTAPPLICABLE, NOTAPPLICABLE));
        }

        void WatchFace(string entity, int face)
        {
            _watches.push_back(SKWatchItem(entity, face, NOTAPPLICABLE));
        }
        void WatchPoint(string entity, int face, int point)
        {
            _watches.push_back(SKWatchItem(entity, face, point));
        }

        void Update(SKModel *model);
};


#endif