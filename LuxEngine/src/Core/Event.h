#pragma once 
#include "Utils/Assert.h"
#include "Utils/Types.h"

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



struct Event
{    
    bool valid = true;
    
    // Event Props
    u8 mod;
    u8 action;
    i16 width;
    i16 height;
    int key_code;
    v2 position, delta;

private:

    EventType p_type;
    bool erasable;
    
public:

    Event(EventType event_type, bool is_erasable = true)
        : p_type(event_type), erasable(is_erasable)
    {
    }

    inline void reset() 
    { 
        valid = false; 
        mod = 0;
        action = 0;
        width = 0;
        height = 0;
        key_code = 0;
        position = { 0, 0};
        delta = { 0, 0};
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