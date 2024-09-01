#ifndef __EVENT_LISTENER__
#define __EVENT_LISTENER__

#include <string>
using namespace std;    

#define STR_KEYDOWN "keydown"
#define STR_KEYUP "keyup"

enum class KeyState
{
    KeyDown,
    KeyUp
};

enum class MouseState
{
    LeftButton,
    RightButton
};

typedef struct _KeyEvent
{
    string key;
    KeyState state;
} KeyEvent;

bool IsEvent(string name);

class EventListener
{
    public:
        virtual void KeyEvent(string event, KeyState state) = 0;
        virtual void MouseEvent(float x, float y, MouseState state) = 0;
};

#endif