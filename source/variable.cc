#include "variable.h"
#include <sstream>

int SKVariable::_nameCount = 0;

void SKVariable::SetName(string name)
{
    _name = name;
    if(_name == "")
    {
        stringstream s;
        s << "__" << SKVariable::_nameCount++ << "__tmp";
        _name = s.str();
    }
}

string SKVariable::GetName()
{
    return _name;
}

void SKVariable::SetType(string type)
{
    _type = type;
}

string SKVariable::GetType()
{
    return _type;
}
