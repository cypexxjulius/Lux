#pragma once 
#include "Utils/Assert.h"
#include "Utils/Types.h"

#include "Keycode.h"

namespace Lux
{

enum class EventType : u8
{
    Char,
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

struct EventCore 
{
public:

    inline void activate()
    { active = true;}

    inline void disable()
    { active = false; }

    inline bool is_active()
    { return active; }

private:

    bool active{ true };

public:

    float delta_time;

};

template<EventType Type>
struct Event
{
private:
    Event() = default;
};

template<>
struct Event<EventType::Char> : public EventCore
{
    Event(u32 event_char)
    {
        character = event_char;
    }

public:
    u32 character;
};


template<>
struct Event<EventType::Scrolled> : public EventCore
{
    v2 delta;
};

template<>
struct Event<EventType::KeyPressed> : public EventCore
{

    Event(Key event_key, KeyState event_state, u8 event_mod)
    :   key(event_key), action(event_state), mod(event_mod)
    {
    }

public:
    Key key;
    KeyState action;
    u8 mod;
};

template<>
struct Event<EventType::MouseMoved> : public EventCore
{
    v2 delta;
    v2 position;
};

template<>
struct Event<EventType::WindowResize> : public EventCore
{
public:

    float width, height;

};

template<>
struct Event<EventType::MouseButtonPressed> : public EventCore
{

    Event(MouseKey event_key, KeyState event_state, u8 event_mod, v2 event_position)
    :   key(event_key), state(event_state), mod(event_mod), position(event_position)
    {}

public:
    MouseKey key;
    KeyState state;
    u8 mod;
    v2 position;

};

}