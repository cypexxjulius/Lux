#pragma once 


#include "Core/Layer.h"
#include "Core/Event.h"

#include "Utils/Types.h"
#include "Utils/Assert.h"

#include "Graphics/CameraController.h"

#include <vector>


#include "GUI.h"
#include "LGA.h"

namespace Lux::GUI
{

class GUILayer final : public Layer
{

private:

    static GUILayer* s_Instance;
    std::vector<GUI::Box> m_boxes;

    Camera2DController m_camera;


    static constexpr GUILayer& Get()
    { return *s_Instance; }

    static constexpr bool Valid()
    { return s_Instance != nullptr; }


public:

    friend class LGA;

    virtual void on_attach() override
    {
        Verify(s_Instance == nullptr);

        s_Instance = this;
    }   

    virtual void on_detach() override 
    {
        s_Instance = nullptr;
    }

    virtual bool on_event(const Event& event) override;

    virtual void on_update() override;


};

}