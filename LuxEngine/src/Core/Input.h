#pragma once 


#include "Application.h"


namespace Lux 
{


class Input 
{

public:

    static constexpr KeyState GetState(Key key)
    { return Application::Get().m_iostate.keyboard[static_cast<u32>(key)]; }

    static constexpr KeyState GetState(MouseKey key)
    { return Application::Get().m_iostate.mouse_buttons[static_cast<u32>(key)]; }
    
};

}