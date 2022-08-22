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

class SystemController
{
public:

  
    static constexpr SystemOS OS()
    {
    #ifdef LX_WIN

        return SystemOS::Windows; 

    #elif LX_UNIX

        return SystemOS::Linux  

    #elif LX_APPLE

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