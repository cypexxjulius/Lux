#pragma once 

#include "Utils/Types.h"

namespace Lux
{

enum class SystemOS
{
    Windows,
    Linux,
    MacOS
};

enum class SystemRenderer
{
    OpenGL,
    Vulcan,
    DirectX12,
    Metal
};

class SystemController
{
public:

    static SystemRenderer Renderer();


    static constexpr SystemOS OS()
    {
    #ifdef _WIN32

        return SystemOS::Windows; 

    #elif __unix__

        return SystemOS::Linux  

    #elif __APPLE__

        return SystemOS::MacOS
    #endif 
    
    } 

    static constexpr int MaxMouseKeys()
    {
        if constexpr(OS() == SystemOS::Windows)
            return 7; // GLFW_KEY_LAST
    }


    static constexpr int MaxKeyboardKeys()
    {
        if constexpr(OS() == SystemOS::Windows)
            return 348; //GLFW_MOUSE_BUTTON_LAST
    }



};

}