#ifndef __VALUE__
#define __VALUE__

class SKValue
{
    public:
        SKValue() {}
        virtual ~SKValue() {}

        virtual SKValue *AllocCopy() = 0;
};

#endif