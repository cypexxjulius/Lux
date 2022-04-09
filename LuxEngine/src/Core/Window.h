#pragma once 

#include <string>
#include "Utils/Types.h"
#include "Utils/Assert.h"



namespace Lux
{

enum class CursorType;

class Window
{
private:

    Window() = delete;

    static void Open(const std::string& title, u32 width, u32 height);

    static void Close();

    static void SwapBuffers();

    static void SetCursorType(CursorType type);

public:


    friend class Application;

    friend class Input;

    static void PollEvents();

    static void SetVsync(bool state);

    static bool IsVsyncSet();

    static float DeltaTime();


};


}