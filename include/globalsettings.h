#ifndef __GLOBALSTRUCTURES__
#define __GLOBALSTRUCTURES__
#include <string>
using namespace std;

#define CAMERA "c."
#define OBJECT "o."
#define LIGHTING "l."
#define KEY "k."
#define MOUSE "m."

#define INTERACT_SIMPLELIGHTING "simplelighting"
#define INTERACT_ADVANCEDLIGHTING "advancedlighting"
#define INTERACT_POINTS "points"
#define LASTVAR "___LAST___VAR___"

#define MOUSEX "m.x"
#define MOUSEY "m.y"

#define RAW_MOUSEX "m.rawx"
#define RAW_MOUSEY "m.rawy"

 enum class MemberType
{
    ObjectMember,
    CameraMember,
    KeyMember,
    LightingMember,
    MouseMember,
    NoMember
};

class MemberCheck
{
    public:
        static MemberType FindMemberType(string var)
        {
            MemberType returnvalue = MemberType::NoMember;
            if(var.length()>2)
            {
                if(var.substr(0,2)==CAMERA)
                    returnvalue = MemberType::CameraMember;
                else if(var.substr(0,2)==OBJECT)
                    returnvalue = MemberType::ObjectMember;
                else if(var.substr(0,2)==LIGHTING)
                    returnvalue = MemberType::LightingMember;
                else if(var.substr(0,2)==KEY)
                    returnvalue = MemberType::KeyMember;
                else if(var.substr(0,2)==MOUSE)
                    returnvalue = MemberType::MouseMember;
            }
            return returnvalue;
        }
        static string GetVarType(string var)
        {
            string returnvalue = var;
            if(var.length()>2)
                returnvalue = var.substr(0,2);
            return returnvalue;
        }
        static string GetVarMember(string var)
        {
            string returnvalue = var;
            if(var.length()>2)
                returnvalue = var.substr(2);
            return returnvalue;
        }
};

#endif