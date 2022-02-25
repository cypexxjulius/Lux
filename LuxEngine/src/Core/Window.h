#pragma once 

#include <string>
#include "Utils/Types.h"
#include "Utils/Assert.h"

namespace Lux
{

class Window
{
private:
    bool m_vsync;
    u32 m_width, m_height;
    void* m_handle;
    std::string m_title;

public:

    Window(const std::string& title, u32 width, u32 height);

    ~Window();

    void swap_buffers();

    void poll_events();

    void set_vsync(bool state);

    bool is_vsync_set();

};


}