#include <Lux.h>
#include <iostream>
#include <memory>

using namespace Lux;

class EngineLayer final : public Lux::Layer
{
private:
    Camera2DController camera_obj{};
    v2 text_dimemsions{};

    GUI::BoxHandle m_boxHandle;

public:

    void on_attach() override
    {
        m_boxHandle = GUI::CreateBox("Test", { 0.0f, 0.0f });

    }

    void on_detach() override
    {
        GUI::Delete({
            m_boxHandle
        });
    }

    void on_update() override
    {
        
    }

};



void lux_start()
{
    Lux::Application::PushLayer<EngineLayer>();
}