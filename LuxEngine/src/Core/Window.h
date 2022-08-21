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

    void* m_Assets = nullptr;

public:

    Window(const std::string& title, u32 width, u32 height);

    ~Window();

    void swap_buffers();

    void set_cursor_type(CursorType type);

    void poll_events();

    void set_vsync(bool state);

    float delta_time();


};


}