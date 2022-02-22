#pragma once 

#include <vector>
#include <string>

#include "Layer.h"
#include "Utils/Types.h"

namespace Lux
{


// Virtual representation of the keyboard and mouse state, updated and owned by the Application class
struct VirtualIO
{
    
    v2 mouse_position;
    u32 mouse_buttons[32];
    u32 keyboard[256];


    // FIXME: Add controller support 
};


class Application
{

private:

    static Application* s_Instance;
    mutable std::vector<Layer*> m_layerstack;
    std::string m_title;
    
    bool minimized = false;
    bool shutdownState = false;
    u16 width = 1280; 
    u16 height = 720;

    VirtualIO io_state;

public:

    Application(const std::string& title);

    static inline const Application& Get() 
    { return *s_Instance; } 


    void loop();

    template<class T>
    inline void push_layer() const
    { 
        static_assert(std::is_base_of<Layer, T>::value, "Class has to inherit from the Layer class");
        m_layerstack.emplace_back(new T);  
        m_layerstack.back()->on_attach();

    }


};



}