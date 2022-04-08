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


namespace Lux::GUI
{

class GUILayer final : public Layer
{

private:

    static GUILayer* s_Instance;

    Box* m_selected_box = nullptr;
    Box* m_dragged_box = nullptr;
    Box* m_active_box = nullptr;

    std::vector<Box*> m_box_positions;
    std::unordered_map<std::string, Box> m_boxes;


    Camera2DController m_camera;
    Ref<Font> m_used_font;


    static inline GUILayer& Get()
    { return *s_Instance; }

    static constexpr bool Valid()
    { return s_Instance != nullptr; }


public:

    friend class LGA;

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

    virtual bool on_event(const Event& event) override;

    virtual void on_update() override;


};

}