#ifndef __TRANSITION__
#define __TRANSITION__
#include <string>
#include <list>
using namespace std;

class SKEntity;
class SKAction;
class SKTransition
{
    public:
        SKTransition() {}
        virtual ~SKTransition() {}

        virtual SKTransition *AllocCopy() = 0;
        virtual void PrepareForAction(SKAction *action, string parentState) = 0;
        virtual bool Evaluate(SKEntity *entity, string& nextState) = 0;
};


#endif