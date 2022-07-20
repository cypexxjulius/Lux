#pragma once 

#include "GUI.h"

#include "Core/Layer.h"
#include "Core/Event.h"

#include "Utils/Types.h"
#include "Utils/Assert.h"

#include "Graphics/Camera/Camera2D.h"

#include "Graphics/Camera/Camera3D.h"

#include <vector>
#include <string>

#include "Assets/Manager.h"
#include "Types.h"

namespace Lux::GUI
{

class GUILayer final : public Layer
{

private:

    static GUILayer* s_Instance;

    std::unordered_map<std::string, Box> m_box_container;
    std::vector<Box*> m_box_positions;


    Camera2D m_Camera{};
    Camera3D m_Camera3D{45.0f, Application::AspectRatio()};
    Ref<Font> m_UsedFont;

    bool m_pressed = false;

    static inline GUILayer& Get()
    { return *s_Instance; }

    static inline bool Valid()
    { return s_Instance != nullptr; }


public:

    virtual void on_attach() override
    {
        Verify(s_Instance == nullptr);
        s_Instance = this;

        // TODO Check if this font exists
        m_UsedFont = ResourceManager::CreateFont("StandardFont", { "res/fonts/Roboto-Medium.ttf" });
    }   

    virtual void on_detach() override 
    {
        s_Instance = nullptr;
    }

    virtual bool on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event) override;

    virtual bool on_mouse_move(const Event<EventType::MouseMoved>& event) override;

    virtual void on_resize(const Event<EventType::WindowResize>& event) override;

    virtual bool on_scroll(const Event<EventType::Scrolled>& event) override;

    virtual void on_update() override;


};

}