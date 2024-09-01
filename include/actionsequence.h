#ifndef __ACTIONSEQUENCE__
#define __ACTIONSEQUENCE__
#include <string>
#include <vector>
using namespace std;

class SKAssignment;
class SKTransition;
class SKConsume;
class SKModel;
class SKAction;
class SKPunchTicket;
class SKEntity;
class SKActionThread;
class SKContext;
class SKActionSequence
{
    private:
        string _name;
        SKModel *_model;
        SKAction *_parent;
        vector<SKAssignment *> _assignments;
        vector<SKTransition *> _transitions;
        vector<SKConsume *> _consumers;
        vector<SKActionThread *> _threads;
        vector<SKPunchTicket *> _ticketpunches;
    public:
        SKActionSequence(SKModel *model, string name);
        string Update(SKEntity *attached, SKContext *context, string ticket);
        void PrepareForAction(SKAction *parent);
        virtual ~SKActionSequence();
        string GetName();
        void AddAssignment(SKAssignment* assignment);
        void AddTransition(SKTransition *transition);
        void AddConsumer(SKConsume *consumer);
        void AddActionThread(SKActionThread *thread);
        void AddTicketPunch(SKPunchTicket *ticketPunch);
        SKActionSequence *AllocCopy();
};
#endif