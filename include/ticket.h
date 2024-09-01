#ifndef __TICKET__
#define __TICKET__

#include <string>
using namespace std;

class SKModel;
class SKPunchTicket
{
    private:
        string _ticketname;
        SKModel *_model;

    public:
        SKPunchTicket(string ticketname, SKModel *model)
        {
            _ticketname = ticketname;
            _model = model;
        }
        SKPunchTicket *AllocCopy()
        {
            SKPunchTicket *returnvalue = new SKPunchTicket(_ticketname, _model);
            return returnvalue;
        }
        void Punch();
};

class SKCheckTicket
{
    public:
        static bool Check(string ticketname, SKModel *model);
};

class SKClearTicket
{
    public:
        static void Clear(string ticketname, SKModel *model);
};


#endif