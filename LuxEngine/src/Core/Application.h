#pragma once 

#include <vector>
#include <string>
#include <memory>

#include "Layer.h"
#include "Window.h"
#include "Utils/Types.h"
#include "Event.h"

#include "Graphics/Renderer.h"
#include "System/SystemController.h"

namespace Lux
{


// Virtual representation of the keyboard and mouse state, updated and owned by the Application class
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

};

class Application
{

private:

    bool m_minimized = false;
    bool m_running = true;
    float m_width = 1280; 
    float m_height = 720;
    float m_frame_time;
    float m_aspect_ratio;

    std::string m_title;
    static Application* s_Instance;
    mutable std::vector<Layer*> m_layerstack{};





    Window m_window;
    
    EventBuffer m_event_buffer{};
    VirtualIO m_iostate{};



private:

    void prepare_event_buffer();

    EventBuffer& get_event_buffer(); 

    void dispatch_event_buffer();

    void dispatch_single_event(Event& event);

private:

    static constexpr Application& Get()
    { return *s_Instance; }

    inline void close_application()
    { m_running = false; }


public:

    friend class Window;

    friend class Input;

    Application(const std::string& title);

    void loop();


    template<class T>
    static inline void PushLayer()
    { 
        static_assert(std::is_base_of<Layer, T>::value, "Class has to inherit from the Layer class");
        Application::Get().m_layerstack.emplace_back(new T);  
        Application::Get().m_layerstack.back()->on_attach();

    }

    static inline float Width()
    { return Application::Get().m_width; }

    static inline float Height()
    { return Application::Get().m_height; }

    static inline float AspectRatio()
    { return Application::Get().m_aspect_ratio; }

    static inline VirtualIO& IOState()
    { return Application::Get().m_iostate; }

};



}