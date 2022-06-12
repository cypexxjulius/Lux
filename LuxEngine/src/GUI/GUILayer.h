#pragma once 

#include "GUI.h"

#include "Core/Layer.h"
#include "Core/Event.h"

#include "Utils/Types.h"
#include "Utils/Assert.h"

#include "Graphics/CameraController.h"

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


    Camera2DController m_camera;
    Ref<Font> m_used_font;


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
        m_used_font = ResourceManager::GetFont("StandardFont");
    }   

    virtual void on_detach() override 
    {
        s_Instance = nullptr;
    }

    virtual bool on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event) override;

    virtual bool on_mouse_move(const Event<EventType::MouseMoved>& event) override;

    virtual bool on_resize(const Event<EventType::WindowResize>& event) override;

    virtual void on_update() override;


};

}