#pragma once 

#include "Utils/Types.h"
#include "Core/Keycode.h"

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
            return (static_cast<int>(MouseKey::MAX) - 4);
    }


    static constexpr int MaxKeyboardKeys()
    {
        if constexpr(OS() == SystemOS::Windows)
            return static_cast<int>(Key::MAX);
    }



};

}