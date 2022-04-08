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

    Event scrolled{EventType::Scrolled};
    Event mouse_moved{EventType::MouseMoved};
    Event window_resize{EventType::WindowResize, false};

    std::vector<Event> mouse_button;
    std::vector<Event> key_events;
    std::vector<Event> char_events;

    friend class Application;

private:

    void prepare(float frame_time)
    {
        scrolled.reset(frame_time);
        mouse_moved.reset(frame_time);
        window_resize.reset(frame_time);

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

    static void DispatchSingleEvent(Event& event);

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
    static inline void PushLayer()
    { 
        static_assert(std::is_base_of<Layer, T>::value, "Class has to inherit from the Layer class");
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