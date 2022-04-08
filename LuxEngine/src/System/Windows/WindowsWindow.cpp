#include "Core/Application.h"
#include "Core/Keycode.h"
#include "Core/Window.h"
#include "Core/Event.h"

#include "Utils/Logger.h"
#include "Utils/Assert.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Lux
{

constexpr KeyState gl_to_lux_keystate[] = {
    KeyState::Released,
    KeyState::Pressed,
    KeyState::Repeated
};


Key OpenGL_to_Lux_KeyCode(GLenum gl_key);
MouseKey OpenGL_to_Lux_MouseKey(GLenum gl_key); 

Window::Window(const std::string& title, u32 width, u32 height)
    : m_width(width), m_height(height), m_title(title)
{


    Verify(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    m_handle = (void*)glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    glfwMakeContextCurrent((GLFWwindow*)m_handle);

    Verify(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress));

    glfwSetWindowUserPointer((GLFWwindow*)m_handle, this);

    glfwSwapInterval(1);
    m_vsync = true;

    GLFWwindow* m_Handle = (GLFWwindow*)m_handle;

#pragma warning(disable: 4100)
    glfwSetFramebufferSizeCallback(m_Handle, [](GLFWwindow* glWindow, int width, int height)
    {

        auto& app = Application::Get();
        auto& buffer = app.m_event_buffer; 
        app.m_width  = buffer.window_resize.width  = static_cast<float>(width);
        app.m_height = buffer.window_resize.height = static_cast<float>(height);
        app.m_aspect_ratio = app.m_width / app.m_height;

        buffer.window_resize.valid = true;

        app.m_minimized = (width + height) == 0;
    });

    // Window close callback 
    glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* glWindow)
    {    
        Application::Get().close_application();
    });

    // Char input callback
    glfwSetCharCallback(m_Handle, [](GLFWwindow* glWindow, unsigned int character)
    {
        auto& buffer = Application::Get().m_event_buffer; 
        buffer.char_events.emplace_back(EventType::Char);
    });

    // General key interaction callback
    glfwSetKeyCallback(m_Handle, [](GLFWwindow* glWindow, int key, int scancode, int action, int mods)
    {
        auto& app = Application::Get();
        auto& buffer = app.m_event_buffer; 
        Event event(EventType::KeyPressed);
        event.action = gl_to_lux_keystate[static_cast<u8>(action)];
        event.keycode.keyboard = OpenGL_to_Lux_KeyCode(key);
        event.mod = static_cast<u8>(mods);
        
        app.m_iostate.keyboard[static_cast<u32>(event.keycode.keyboard)] = event.action;

        buffer.key_events.emplace_back(event);    

    });

    // Mouse button interaction callback
    glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* glWindow, int button, int action, int mods)
    { 
        auto& app = Application::Get();
        auto& buffer = app.m_event_buffer; 
        
        Event event(EventType::MouseButtonPressed);
        event.keycode.mouse = OpenGL_to_Lux_MouseKey(button);
        event.mod = static_cast<u8>(mods);
        event.action = gl_to_lux_keystate[static_cast<u8>(action)];
        event.position = app.m_iostate.mouse_position;

        app.IOState().mouse_buttons[static_cast<u32>(event.keycode.mouse)] = event.action;
        
        buffer.mouse_button.emplace_back(event);
    });

    glfwSetScrollCallback(m_Handle, [](GLFWwindow* glWindow, double xOffset, double yOffset)
    {
        auto& buffer = Application::Get().m_event_buffer; 
        buffer.scrolled.delta += v2{ xOffset, yOffset};
        buffer.scrolled.valid = true;
    }); 

    glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* glWindow, double x, double y)
    {
        static double xOldPos = 0.0, yOldPos = 0.0; 
        auto& app = Application::Get();
        auto& buffer = app.m_event_buffer; 

        x /= static_cast<double>(Application::Width());

        y /= static_cast<double>(Application::Height());


        buffer.mouse_moved.delta += v2{ x - xOldPos, y - yOldPos };
        buffer.mouse_moved.position =  v2{ x, y };

        buffer.mouse_moved.valid = true;

        app.m_iostate.mouse_position = buffer.mouse_moved.position;

        xOldPos = x;
        yOldPos = y;
    });
#pragma warning(default: 4100)

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

float Window::delta_time()
{
    static double lastTime = 0.0;

    double time = (float)glfwGetTime(); 
    double deltaTime = time - lastTime;
    lastTime = time;
    return (float)deltaTime;
}

};