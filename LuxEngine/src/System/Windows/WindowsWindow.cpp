#include "Core/Keycode.h"
#include "Core/Window.h"
#include "Core/Event.h"

#include "Utils/Logger.h"
#include "Utils/Assert.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Input.h"

#include "Core/Application.h"

namespace Lux
{

constexpr KeyState glfw_to_lux_keystate[] = {
    KeyState::Released,
    KeyState::Pressed,
    KeyState::Repeated
};

Key glfw_to_lux_keycode(GLenum gl_key);
MouseKey glfw_to_lux_mousekey(GLenum gl_key); 

struct WindowAssets
{
    bool vsync;
    GLFWwindow* handle;

    std::array<GLFWcursor*, 9> cursors;
};


Window::Window(const std::string& title, u32 width, u32 height)
{
    auto wAssets = new WindowAssets;
    m_Assets = wAssets;

    Verify(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    glfwWindowHint(GLFW_SAMPLES, 4);


    wAssets->handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    glfwMakeContextCurrent(wAssets->handle);

    Verify(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress));

    glfwSwapInterval(1);
    wAssets->vsync = true;

    wAssets->cursors[0] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    wAssets->cursors[1] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    wAssets->cursors[2] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    wAssets->cursors[3] = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
    wAssets->cursors[4] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    wAssets->cursors[5] = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    wAssets->cursors[6] = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
    wAssets->cursors[7] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
    wAssets->cursors[8] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);


#pragma warning(disable: 4100)
    glfwSetFramebufferSizeCallback(wAssets->handle, [](GLFWwindow* glWindow, int width, int height)
    {

        auto& buffer = Application::PrivateGet().PrivateGet().m_EventBuffer;
        
        Application::PrivateGet().m_Minimized = (width + height) == 0;
        if (Application::PrivateGet().m_Minimized)
            return;

        Application::PrivateGet().m_Width  = buffer.window_resize.width  = static_cast<float>(width);
        Application::PrivateGet().m_Height = buffer.window_resize.height = static_cast<float>(height);
        Application::PrivateGet().m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);

        buffer.window_resize.activate();

    });

    // Window close callback 
    glfwSetWindowCloseCallback(wAssets->handle, [](GLFWwindow* glWindow)
    {    
        Application::PrivateGet().close();
    });

    // Char input callback
    glfwSetCharCallback(wAssets->handle, [](GLFWwindow* glWindow, unsigned int character)
    {
        Application::PrivateGet().PrivateGet().m_EventBuffer.char_events.emplace_back( character );
    });

    // General key interaction callback
    glfwSetKeyCallback(wAssets->handle, [](GLFWwindow* glWindow, int key, int scancode, int action, int mods)
    {
        auto& buffer = Application::PrivateGet().m_EventBuffer; 

        Key lux_keycode = glfw_to_lux_keycode(key);
        KeyState lux_keystate = glfw_to_lux_keystate[static_cast<u8>(action)];

        buffer.key_events.emplace_back(
            lux_keycode,
            lux_keystate,
            static_cast<u8>(mods)
        );
        
        Application::PrivateGet().m_InputBuffer.keyboard[static_cast<u32>(lux_keycode)] = lux_keystate;


    });

    // Mouse button interaction callback
    glfwSetMouseButtonCallback(wAssets->handle, [](GLFWwindow* glWindow, int button, int action, int mods)
    { 
        auto& buffer = Application::PrivateGet().m_EventBuffer; 

        MouseKey key = glfw_to_lux_mousekey(button);
        KeyState state = glfw_to_lux_keystate[static_cast<u8>(action)];

        buffer.mouse_button.emplace_back(
            key,
            state,
            static_cast<u8>(mods),
            Application::PrivateGet().m_InputBuffer.mouse_position
        );

        Application::PrivateGet().m_InputBuffer.mouse_buttons[static_cast<u32>(key)] = state;
        
    });

    glfwSetScrollCallback(wAssets->handle, [](GLFWwindow* glWindow, double xOffset, double yOffset)
    {
        auto& buffer = Application::PrivateGet().m_EventBuffer; 
        buffer.scrolled.delta += v2{ xOffset, yOffset};
        
        buffer.scrolled.activate();
    }); 

    glfwSetCursorPosCallback(wAssets->handle, [](GLFWwindow* glWindow, double x, double y)
    {
        static double xOldPos = 0.0, yOldPos = 0.0; 
        auto& buffer = Application::PrivateGet().m_EventBuffer; 

        x /= static_cast<double>(Application::Width());

        y /= static_cast<double>(Application::Height());


        buffer.mouse_moved.delta += v2{ x - xOldPos, y - yOldPos };
        buffer.mouse_moved.position =  v2{ x, y };

        buffer.mouse_moved.activate();

        Application::PrivateGet().m_InputBuffer.mouse_position = buffer.mouse_moved.position;

        xOldPos = x;
        yOldPos = y;
    });
#pragma warning(default: 4100)

}

void Window::swap_buffers()
{
    glfwSwapBuffers(static_cast<WindowAssets*>(m_Assets)->handle);
}

void Window::poll_events()
{
    glfwPollEvents();
}

void Window::set_vsync(bool state)
{
    static_cast<WindowAssets*>(m_Assets)->vsync = state;
    glfwSwapInterval((int)state);
}

Window::~Window()
{
    for(u32 i = 0; i < 6; i++)
    {
        glfwDestroyCursor(static_cast<WindowAssets*>(m_Assets)->cursors[i]);
    }
 
    glfwDestroyWindow(static_cast<WindowAssets*>(m_Assets)->handle);
    glfwTerminate();
    
    delete static_cast<WindowAssets*>(m_Assets);
}

void Window::set_cursor_type(CursorType type)
{
    static CursorType static_type = CursorType::Arrow;
    if(type == static_type)
        return;
    
    static_type = type;
    glfwSetCursor(static_cast<WindowAssets*>(m_Assets)->handle, static_cast<WindowAssets*>(m_Assets)->cursors[static_cast<int>(type)]);
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