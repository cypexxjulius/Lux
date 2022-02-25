#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Event.h"
#include "Utils/Assert.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Lux
{


Window::Window(const std::string& title, u32 width, u32 height)
    : m_width(width), m_height(height), m_title(title)
{


    Verify(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    m_handle = (void*)glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    glfwMakeContextCurrent((GLFWwindow*)m_handle);

    Verify(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress));

    glfwSetWindowUserPointer((GLFWwindow*)m_handle, this);

    glfwSwapInterval(1);
    m_vsync = true;

    GLFWwindow* m_Handle = (GLFWwindow*)m_handle;

    glfwSetFramebufferSizeCallback(m_Handle, [](GLFWwindow* glWindow, int width, int height)
    {
        auto& app = Application::Get_private();
        auto& buffer = app.m_event_buffer; 
        app.m_width  = buffer.window_resize.width  = width;
        app.m_height = buffer.window_resize.height = height;

        buffer.window_resize.valid = true;

        app.m_minimized = (width + height) == 0;
    });

    // Window close callback 
    glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* glWindow)
    {    
        Application::Get_private().close_application();
    });

    // Char input callback
    glfwSetCharCallback(m_Handle, [](GLFWwindow* glWindow, unsigned int character)
    {
        auto& buffer = Application::Get_private().m_event_buffer; 
        buffer.char_events.emplace_back(Event(EventType::Char));
    });

    // General key interaction callback
    glfwSetKeyCallback(m_Handle, [](GLFWwindow* glWindow, int key, int scancode, int action, int mods)
    {
        auto& app = Application::Get_private();
        auto& buffer = app.m_event_buffer; 
        Event event(EventType::KeyPressed);
        event.action = action;
        event.key_code = key;
        event.mod = mods;
        
        buffer.key_events.emplace_back(std::move(event));    

        app.m_iostate.keyboard[key] = action;
    });

    // Mouse button interaction callback
    glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* glWindow, int button, int action, int mods)
    { 
        auto& app = Application::Get_private();
        auto& buffer = app.m_event_buffer; 
        
        Event event(EventType::MouseButtonPressed);
        event.key_code = button;
        event.mod = mods;
        event.action = action;
        event.position = app.m_iostate.mouse_position;

        buffer.mouse_button.emplace_back(std::move(event));
    });

    glfwSetScrollCallback(m_Handle, [](GLFWwindow* glWindow, double xOffset, double yOffset)
    {
        auto& buffer = Application::Get_private().m_event_buffer; 
        
        buffer.scrolled.delta = v2{ xOffset, yOffset};
        buffer.scrolled.valid = true;
    }); 

    glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* glWindow, double x, double y)
    {
        static double xOldPos = 0.0, yOldPos = 0.0; 
        auto& app = Application::Get_private();
        auto& buffer = app.m_event_buffer; 


        buffer.mouse_moved.delta += v2{ x - xOldPos, y - yOldPos };
        buffer.mouse_moved.position =  v2{ x, y };

        buffer.mouse_moved.valid = true;

        app.m_iostate.mouse_position = buffer.mouse_moved.position;

        xOldPos = x;
        yOldPos = y;
    });

}

void Window::swap_buffers()
{
    glfwSwapBuffers((GLFWwindow*)m_handle);
}

void Window::poll_events()
{
    glfwPollEvents();
}

void Window::set_vsync(bool state)
{
    m_vsync = state;
    glfwSwapInterval((int)state);
}

Window::~Window()
{
    glfwDestroyWindow((GLFWwindow*)m_handle);
    glfwTerminate();
}


};