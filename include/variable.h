#include <string>
#include "entity.h"
using namespace std;

#define STRING_TYPE "string"
#define NUMBER_TYPE "number"
#define STATE_TYPE "state"

#ifndef __VARIABLE__
#define __VARIABLE__

class SKVariable
{
    private:
        string _name;
        string _type;
        static int _nameCount;

    protected:
        void SetType(string type);
        void SetName(string name);
    
    public:
        SKVariable() {}
        virtual ~SKVariable() {}
        string GetName();
        string GetType();
        virtual SKVariable *AllocCopy() {return NULL;}
        virtual string DebugString() = 0;

};

#endif
