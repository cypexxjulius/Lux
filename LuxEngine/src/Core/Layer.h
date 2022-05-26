#pragma once 

#include "Event.h"

namespace Lux 
{

class Layer
{
protected:
    Layer() = default;
public:
    
    virtual ~Layer() {}

    virtual void on_attach() = 0;

    virtual void on_detach() = 0;

    virtual void on_update() = 0;

    virtual bool on_char_press(const Event<EventType::Char>& event) 
    {
        (void) event;
        return false;
    }

    virtual bool on_key_press(const Event<EventType::KeyPressed>& event)
    {
        (void) event;
        return false;
    }
    
    virtual bool on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event)
    {
        (void) event;
        return false;
    }
    
    virtual bool on_mouse_move(const Event<EventType::MouseMoved>& event)
    {
        (void) event;
        return false;
    }

    virtual bool on_scroll(const Event<EventType::Scrolled>& event)
    {
        (void) event;
        return false;
    }

    virtual bool on_resize(const Event<EventType::WindowResize>& event)
    {
        (void) event;
        return false;
    }
    
};


}