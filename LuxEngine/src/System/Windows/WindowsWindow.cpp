#include "Core/Keycode.h"
#include "Core/Window.h"
#include "Core/Event.h"

#include "Utils/Logger.h"
#include "Utils/Assert.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Input.h"


namespace Lux
{

constexpr KeyState glfw_to_lux_keystate[] = {
    KeyState::Released,
    KeyState::Pressed,
    KeyState::Repeated
};

Key GLFW_to_Lux_KeyCode(GLenum gl_key);
MouseKey GLFW_to_Lux_MouseKey(GLenum gl_key); 

struct WindowAssets
{
    bool vsync;
    GLFWwindow* handle;

    std::array<GLFWcursor*, 9> cursors;
};

static WindowAssets* wAssets = nullptr;

void Window::Open(const std::string& title, u32 width, u32 height)
{
    Verify(wAssets == nullptr);

    wAssets = new WindowAssets;

    Verify(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

        auto& buffer = Application::m_EventBuffer; 
        Application::m_Width  = buffer.window_resize.width  = static_cast<float>(width);
        Application::m_Height = buffer.window_resize.height = static_cast<float>(height);
        Application::m_AspectRatio = Application::m_Width / Application::m_Height;

        buffer.window_resize.valid = true;

        Application::m_Minimized = (width + height) == 0;
    });

    // Window close callback 
    glfwSetWindowCloseCallback(wAssets->handle, [](GLFWwindow* glWindow)
    {    
        Application::Close();
    });

    // Char input callback
    glfwSetCharCallback(wAssets->handle, [](GLFWwindow* glWindow, unsigned int character)
    {
        Application::m_EventBuffer.char_events.emplace_back(EventType::Char);
    });

    // General key interaction callback
    glfwSetKeyCallback(wAssets->handle, [](GLFWwindow* glWindow, int key, int scancode, int action, int mods)
    {
        auto& buffer = Application::m_EventBuffer; 
        Event event(EventType::KeyPressed);
        event.action = glfw_to_lux_keystate[static_cast<u8>(action)];
        event.keycode.keyboard = GLFW_to_Lux_KeyCode(key);
        event.mod = static_cast<u8>(mods);
        
        Application::m_VirtualIO.keyboard[static_cast<u32>(event.keycode.keyboard)] = event.action;

        buffer.key_events.emplace_back(event);    

    });

    // Mouse button interaction callback
    glfwSetMouseButtonCallback(wAssets->handle, [](GLFWwindow* glWindow, int button, int action, int mods)
    { 
        auto& buffer = Application::m_EventBuffer; 
        
        Event event(EventType::MouseButtonPressed);
        event.keycode.mouse = GLFW_to_Lux_MouseKey(button);
        event.mod = static_cast<u8>(mods);
        event.action = glfw_to_lux_keystate[static_cast<u8>(action)];
        event.position = Application::m_VirtualIO.mouse_position;

        Application::m_VirtualIO.mouse_buttons[static_cast<u32>(event.keycode.mouse)] = event.action;
        
        buffer.mouse_button.emplace_back(event);
    });

    glfwSetScrollCallback(wAssets->handle, [](GLFWwindow* glWindow, double xOffset, double yOffset)
    {
        auto& buffer = Application::m_EventBuffer; 
        buffer.scrolled.delta += v2{ xOffset, yOffset};
        buffer.scrolled.valid = true;
    }); 

    glfwSetCursorPosCallback(wAssets->handle, [](GLFWwindow* glWindow, double x, double y)
    {
        static double xOldPos = 0.0, yOldPos = 0.0; 
        auto& buffer = Application::m_EventBuffer; 

        x /= static_cast<double>(Application::Width());

        y /= static_cast<double>(Application::Height());


        buffer.mouse_moved.delta += v2{ x - xOldPos, y - yOldPos };
        buffer.mouse_moved.position =  v2{ x, y };

        buffer.mouse_moved.valid = true;

        Application::m_VirtualIO.mouse_position = buffer.mouse_moved.position;

        xOldPos = x;
        yOldPos = y;
    });
#pragma warning(default: 4100)

}

void Window::SwapBuffers()
{
    Verify(wAssets != nullptr);
    glfwSwapBuffers(wAssets->handle);
}

void Window::PollEvents()
{
    Verify(wAssets != nullptr);
    glfwPollEvents();
}

void Window::SetVsync(bool state)
{
    Verify(wAssets != nullptr);
    wAssets->vsync = state;
    glfwSwapInterval((int)state);
}

void Window::Close()
{
    Verify(wAssets != nullptr);
 
    for(u32 i = 0; i < 6; i++)
    {
        glfwDestroyCursor(wAssets->cursors[i]);
    }
 
    glfwDestroyWindow(wAssets->handle);
    glfwTerminate();
    
    delete wAssets;
}

void Window::SetCursorType(CursorType type)
{
    static CursorType static_type = CursorType::Arrow;
    if(type == static_type)
        return;
    
    static_type = type;
    glfwSetCursor(wAssets->handle, wAssets->cursors[static_cast<int>(type)]);
}

float Window::DeltaTime()
{
    static double lastTime = 0.0;

    double time = (float)glfwGetTime(); 
    double deltaTime = time - lastTime;
    lastTime = time;
    return (float)deltaTime;
}

};