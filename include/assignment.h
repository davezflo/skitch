#include <string>

using namespace std;

#ifndef __ASSIGNMENT__
#define __ASSIGNMENT__

enum class Operation
{
    Assign,
    Plus,
    Minus,
    Times,
    Divide

};

class SKRValue;
class SKLValue;
class SKEntity;
class SKAction;
class SKAssignment
{
    private:
        SKLValue *_lValue;
        SKRValue *_rValue;
        Operation _op;
        SKAction *_action;
    public:
        SKAssignment();
        virtual ~SKAssignment();
        SKAssignment *AllocCopy();
        void SetLValue(SKLValue *value);
        void SetRValue(SKRValue *value);
        void SetOperation(Operation op);
        void Operate(SKEntity *entity);
};

#endif