#include "ticket.h"
#include "numvar.h"
#include "variable.h"
#include "model.h"
#include "ticket.h"
#include "context.h"

void SKPunchTicket::Punch()
{
    SKVariable *v = _model->GetCurrentContext()->GetVariable(_ticketname);
    ((SKNumberVariable *)v)->SetValue(1.0);
}

bool SKCheckTicket::Check(string ticket, SKModel *model)
{
    if(ticket == "") return false;
    bool returnvalue = false;
    SKVariable *v = model->GetCurrentContext()->GetVariable(ticket);
    returnvalue = ((SKNumberVariable *)v)->GetValue()==1.0?true:false;
    return returnvalue;
}

void SKClearTicket::Clear(string ticket, SKModel *model)
{
    if(ticket == "") return;
    SKVariable *v = model->GetCurrentContext()->GetVariable(ticket);
    ((SKNumberVariable *)v)->SetValue(0.0);
}