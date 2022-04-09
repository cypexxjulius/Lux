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
    { return Application::m_VirtualIO.keyboard[static_cast<u32>(key)]; }

    static inline KeyState GetState(MouseKey key)
    { return Application::m_VirtualIO.mouse_buttons[static_cast<u32>(key)]; }

    static inline void SetCursorType(CursorType type)
    {
        Window::SetCursorType(type);
    }
    
};

}