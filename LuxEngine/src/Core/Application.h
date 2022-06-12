#pragma once 

#include <vector>
#include <string>
#include <memory>

#include "Layer.h"
#include "Event.h"

#include "Utils/Types.h"

#include "System/SystemController.h"

namespace Lux
{


// Virtual representation of the keyboard and mouse state, updated by the window and owned by the Application class
struct VirtualIO
{
    
    v2 mouse_position;
    KeyState mouse_buttons[SystemController::MaxMouseKeys()];
    KeyState keyboard[SystemController::MaxKeyboardKeys()];
    // FIXME: Add controller support 
};

struct EventBuffer
{

    Event<EventType::Scrolled> scrolled;
    Event<EventType::MouseMoved> mouse_moved;
    Event<EventType::WindowResize> window_resize;

    std::vector<Event<EventType::Char>> char_events;
    std::vector<Event<EventType::KeyPressed>> key_events;
    std::vector<Event<EventType::MouseButtonPressed>> mouse_button;

    friend class Application;

private:

    void prepare()
    {
        scrolled.disable();
        scrolled.delta = {};

        mouse_moved.disable();
        mouse_moved.delta = {};

        window_resize.disable();
        window_resize.width = 0;
        window_resize.height = 0;

        key_events.clear();
        char_events.clear();
        mouse_button.clear();
    }

};

class Application
{

private:

    static float m_Width;
    static float m_Height;
    
    static bool m_Minimized;
    static bool m_Running;
    static float m_FrameTime;
    static float m_AspectRatio;

    static std::string m_Title;

    static std::vector<Layer*>  m_LayerStack;
    static VirtualIO            m_VirtualIO;
    static EventBuffer          m_EventBuffer; 

    static void DispatchEventBuffer();

    template<EventType Type>
    static void DispatchSingleEvent(Event<Type>& event);

    static inline void Close()
    {
        m_Running = false;
    }

public:

    friend class Window;

    friend class Input;

    static void Start(const std::string& title);

    static void Run();

    template<class T> 
    requires std::is_base_of_v<Layer, T>
    static inline void PushLayer()
    { 
        m_LayerStack.emplace_back(new T);  
        m_LayerStack.back()->on_attach();
    }

    static inline float Width()
    { return m_Width; }

    static inline float Height()
    { return m_Height; }

    static inline float AspectRatio()
    { return m_AspectRatio; }

};



}