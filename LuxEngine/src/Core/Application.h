#pragma once 

#include <vector>
#include <string>
#include <memory>

#include "Layer.h"
#include "Window.h"
#include "Utils/Types.h"
#include "Event.h"

#include "Renderer/RendererAPI.h"
#include "System/SystemController.h"

namespace Lux
{


// Virtual representation of the keyboard and mouse state, updated and owned by the Application class
struct VirtualIO
{
    
    v2 mouse_position;
    u32 mouse_buttons[SystemController::MaxMouseKeys()];
    u32 keyboard[SystemController::MaxKeyboardKeys()];


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

    std::string m_title;
    static Application* s_Instance;
    mutable std::vector<Layer*> m_layerstack;





    Window m_window;
    std::unique_ptr<RendererAPI> m_renderer;
    
    EventBuffer m_event_buffer;
    VirtualIO m_iostate;



private:

    void prepare_event_buffer();

    EventBuffer& get_event_buffer(); 

    void dispatch_event_buffer();

    void dispatch_single_event(Event& event);

    static inline Application& Get_private()
    { return *s_Instance; }

    inline void close_application()
    { m_running = false; }


public:

    friend class Window;

    Application(const std::string& title);

    void loop();


    template<class T>
    static inline void PushLayer()
    { 
        static_assert(std::is_base_of<Layer, T>::value, "Class has to inherit from the Layer class");
        Application::Get_private().m_layerstack.emplace_back(new T);  
        Application::Get_private().m_layerstack.back()->on_attach();

    }

    static inline float Width()
    { return Application::Get_private().m_width; }

    static inline float Height()
    { return Application::Get_private().m_height; }

    static inline VirtualIO IOState()
    { return Application::Get_private().m_iostate; }

};



}