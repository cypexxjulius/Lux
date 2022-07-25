#pragma once 

#include "Layer.h"
#include "Event.h"

#include "Utils/Types.h"

#include "System/SystemController.h"

#include "Window.h"

namespace Lux
{


// Virtual representation of the keyboard and mouse state, updated by the window and owned by the Application class
struct InputBuffer
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
    static Application* s_Instance;


    float m_Width = 1280;
    float m_Height = 720;

    bool  m_Minimized = false;
    bool  m_Running = true;
    float m_FrameTime = 0.0f;
    float m_AspectRatio = 1280.0f / 720.0f;

    std::string m_Title;

    mutable std::vector<Layer*>  m_LayerStack;
    InputBuffer          m_InputBuffer;
    EventBuffer          m_EventBuffer; 

    Scope<Window> m_Window;

private:

    friend class Window;
    friend class Input;

    void DispatchEventBuffer();

    template<EventType Type>
    void dispatch_single_event(Event<Type>& event)
    {
        for (auto layer : m_LayerStack)
        {
            bool return_value = false;
            if constexpr (Type == EventType::Char)
                return_value = layer->on_char_press(event);
            else if constexpr (Type == EventType::KeyPressed)
                return_value = layer->on_key_press(event);
            else if constexpr (Type == EventType::MouseButtonPressed)
                return_value = layer->on_mouse_button_press(event);
            else if constexpr (Type == EventType::MouseMoved)
                return_value = layer->on_mouse_move(event);
            else if constexpr (Type == EventType::Scrolled)
                return_value = layer->on_scroll(event);
            else if constexpr (Type == EventType::WindowResize)
                layer->on_resize(event);



            if (return_value)
                return;
        }
    }

    inline void close()
    {
        m_Running = false;
    }

    static inline Application& PrivateGet()
    {
        return *s_Instance;
    }

public:

    Application(const std::string& title);

    ~Application();

    void run();

    template<class T> 
    requires std::is_base_of_v<Layer, T>
    static inline void PushLayer()
    { 
        Application::PrivateGet().m_LayerStack.emplace_back(new T());  
        Application::PrivateGet().m_LayerStack.back()->on_attach();
    }

    inline float width()
    { return m_Width; }

    inline float height()
    { return m_Height; }

    inline float aspect_ratio()
    { return m_AspectRatio; }

    static inline float AspectRatio()
    { return s_Instance->aspect_ratio(); }

    static inline float Width()
    { return s_Instance->width(); }

    static inline float Height()
    { return s_Instance->height(); }


    inline const InputBuffer& input_buffer() const
    {
        return m_InputBuffer;
    }

    inline const EventBuffer& event_buffer() const
    {
        return m_EventBuffer;
    }


    static inline const Application& Get()
    {
        return *s_Instance;
    }
};



}