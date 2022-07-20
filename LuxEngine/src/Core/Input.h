#pragma once 

#include "Application.h"
#include "Window.h"


namespace Lux 
{

enum class CursorType
{
    Arrow,
    IBeam,
    Crosshair,
    PointingHand,
    Hand,
    HResize,
    VResize,
    HVResizeTB,
    HVResizeBT,
};

class Input 
{

public:

    static inline KeyState GetState(Key key)
    { return Application::Get().input_buffer().keyboard[static_cast<u32>(key)]; }

    static inline KeyState GetState(MouseKey key)
    { return Application::Get().input_buffer().mouse_buttons[static_cast<u32>(key)]; }

    static inline void SetCursorType(CursorType type)
    {
        Application::PrivateGet().m_Window->set_cursor_type(type);
    }
    
};

}