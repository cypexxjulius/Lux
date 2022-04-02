#pragma once 
#include "Utils/Assert.h"
#include "Utils/Types.h"

#include "Keycode.h"

namespace Lux
{

enum class EventType : u8
{
    Char,
    Close,
    Scrolled,
    KeyPressed,
    MouseMoved,
    WindowResize,
    MouseButtonPressed,
};

enum class KeyState
{
    Released,
    Pressed,
    Repeated
};

struct Event
{    
    bool valid = true;
    
    // Event Props
    u8 mod;
    float width, height;
    KeyState action;
    union 
    {   
        MouseKey mouse;
        Key keyboard;
    } keycode;

    v2 position;
    v2 delta;
    float delta_time;

private:

    EventType p_type;
    bool erasable;
    
public:

    Event(EventType event_type, bool is_erasable = true)
        : p_type(event_type), erasable(is_erasable)
    {
    }

    inline void reset(float time) 
    { 
        valid = false; 
        mod = 0;
        action = KeyState::Released;
        width = 0;
        height = 0;
        position = { 0, 0};
        delta = {0, 0};
        delta_time = time;
    }

    inline EventType type() const 
    { 
        return p_type;
    }

    inline bool is_erasable() const 
    {
        return erasable;
    }
};

}