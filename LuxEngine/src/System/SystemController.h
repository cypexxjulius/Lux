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
        return SystemOS::Windows; 
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