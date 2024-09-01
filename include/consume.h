#ifndef __CONSUME__
#define __CONSUME__
#include <string>
using namespace std;

class SKRValue;
class SKEntity;
class SKConsume
{
    private:
        SKRValue *_x, *_y, *_z;
        float stickyx;
        float stickyy;
        float stickyz;
        string _border, _fill;
        enum class ConsumeType
        {
            ConsumePoint,
            ConsumeFace,
            ConsumeMousePoint,
            ConsumeUndo
        };

        ConsumeType _type;

    public:
        SKConsume();
        virtual ~SKConsume() {}

        SKConsume *AllocCopy();

        void ConsumePoint(SKRValue *x, SKRValue *y, SKRValue *z);
        void ConsumeFace(string border, string fill);
        void ConsumeMousePoint();
        void ConsumeUndo();
        void Consume(SKEntity *entity);

};

#endif