#include "Application.h"
#include "Graphics/Renderer.h"
#include "Graphics/Renderer2D.h"

#include "Assets/Manager.h"
#include "GUI/GUILayer.h"

#include "Window.h"

namespace Lux
{

float       Application::m_Width = 1280; 
float       Application::m_Height = 720;

bool  Application::m_Minimized = false;
bool  Application::m_Running = true;
float Application::m_FrameTime = 0.0f;
float Application::m_AspectRatio = 1280.0f / 720.0f;

std::string             Application::m_Title;
std::vector<Layer*>     Application::m_LayerStack;
VirtualIO               Application::m_VirtualIO;
EventBuffer             Application::m_EventBuffer; 


void Application::Start(const std::string& title)
{
    m_Title = title;
    Window::Open(m_Title, (u32)m_Width, (u32)m_Height);

    Renderer::Init(RendererAPI::OpenGL);
    Renderer::SetClearColor({0.3, 0.4, 0.3, 1.0});

    ResourceManager::Init();

    
    Renderer2D::Init();

    PushLayer<GUI::GUILayer>();

}

void Application::Run()
{
    while(m_Running)
    {

        m_FrameTime = Window::DeltaTime();
        
        // Event Processing
        do {
            
            m_EventBuffer.prepare(m_FrameTime);
            Window::PollEvents();

            DispatchEventBuffer();

        } while(m_Minimized);

        Renderer::Clear();

        // Reverse callstack
        for(auto rit = m_LayerStack.rbegin(); rit != m_LayerStack.rend(); rit++)
            (*rit)->on_update();


        Window::SwapBuffers();
    }

    for(auto& layer : m_LayerStack)
        layer->on_detach();

    Renderer2D::Shutdown();
    ResourceManager::Shutdown();
    Renderer::Shutdown();

    Window::Close();
}


void Application::DispatchEventBuffer()
{ 

    if(m_EventBuffer.scrolled.valid)
        DispatchSingleEvent(m_EventBuffer.scrolled);

    if(m_EventBuffer.mouse_moved.valid)
        DispatchSingleEvent(m_EventBuffer.mouse_moved);

    if(m_EventBuffer.window_resize.valid)
    {
        DispatchSingleEvent(m_EventBuffer.window_resize);
        Renderer::SetViewport(static_cast<u32>(m_Width), static_cast<u32>(m_Height));
    }

    for(auto& event : m_EventBuffer.mouse_button)
    {
        event.delta_time = m_FrameTime;
        DispatchSingleEvent(event);
    }

    for(auto& event : m_EventBuffer.key_events)
    {
        event.delta_time = m_FrameTime;
        DispatchSingleEvent(event);
    }

    for(auto& event : m_EventBuffer.char_events)
    {
        event.delta_time = m_FrameTime;
        DispatchSingleEvent(event);
    }
}



void Application::DispatchSingleEvent(Event& event)
{
    for(auto layer : m_LayerStack)
    {
        bool returnValue = layer->on_event(event);

        if(returnValue && event.is_erasable())
            return;
    }
}

}