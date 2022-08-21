#include "Application.h"
#include "Graphics/Renderer.h"
#include "Graphics/Renderer2D/Renderer2D.h"

#include "Assets/AssetManager.h"
#include "GUI/GUILayer.h"

#include "Window.h"

#include "Utils/Assert.h"

#include "Utils/Types.h"

namespace Lux
{

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& title)
{
    Verify(s_Instance == nullptr);

    s_Instance = this;

    m_Title = title;
    m_Window = create_scope<Window>("LuxEngine", 1280, 720);

    Renderer::Init();


    Renderer::get_api().set_clear_color({0.3, 0.4, 0.3, 1.0});
    
    ResourceManager::Init();

    Renderer2D::Init();

    PushLayer<GUI::GUILayer>();
}

Application::~Application()
{
    for (auto& layer : m_LayerStack)
        layer->on_detach();

    Renderer2D::Shutdown();
    ResourceManager::Shutdown();
    Renderer::Shutdown();

    delete m_Window.release();

    s_Instance = nullptr;
}

void Application::run()
{
    while(m_Running)
    {

        m_FrameTime = m_Window->delta_time();
        
        // Event Processing
        do {
            
            m_EventBuffer.prepare();
            m_Window->poll_events();

            DispatchEventBuffer();

        } while(m_Minimized);

        Renderer::get_api().clear();

        // Reverse callstack
        for(auto rit = m_LayerStack.rbegin(); rit != m_LayerStack.rend(); rit++)
            (*rit)->on_update();


        m_Window->swap_buffers();
    }
}


void Application::DispatchEventBuffer()
{ 

    if(m_EventBuffer.scrolled.is_active())
        dispatch_single_event(m_EventBuffer.scrolled);

    if(m_EventBuffer.mouse_moved.is_active())
        dispatch_single_event(m_EventBuffer.mouse_moved);

    if(m_EventBuffer.window_resize.is_active())
    {
        dispatch_single_event(m_EventBuffer.window_resize);
        Renderer::get_api().set_viewport(static_cast<u32>(m_Width), static_cast<u32>(m_Height));
    }

    for(auto& event : m_EventBuffer.mouse_button)
    {
        event.delta_time = m_FrameTime;
        dispatch_single_event(event);
    }

    for(auto& event : m_EventBuffer.key_events)
    {
        event.delta_time = m_FrameTime;
        dispatch_single_event(event);
    }

    for(auto& event : m_EventBuffer.char_events)
    {
        event.delta_time = m_FrameTime;
        dispatch_single_event(event);
    }
}




}