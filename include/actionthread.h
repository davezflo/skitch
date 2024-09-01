#include <string>
#include <vector>
#include "globalsettings.h"
using namespace std;

class SKValue;
class SKEntity;
class SKModel;
class SKRValue;
class SKActionThread
{
    private:
        string _actionname;
        MemberType _membertype;
        string _membervariable;
        vector<SKRValue *> _values;
        string _waiton;
        string _ticket;
    public:
        SKActionThread(string name, MemberType memberType, 
                        string memberVariable, vector<SKRValue *> values, 
                        string waiton, string ticket);
        virtual ~SKActionThread();
        void Execute(SKModel* model, SKEntity *entity);
        SKActionThread *AllocCopy();

};