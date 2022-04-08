#include <Lux.h>
#include <iostream>
#include <memory>

using namespace Lux;

class EngineLayer final : public Lux::Layer
{
private:
    Camera2DController camera_obj{};
    v2 text_dimemsions{};

public:


    EngineLayer() 
    {

    }

    void on_attach() override
    {

    }

    void on_detach() override
    {

    }

    bool on_event(const Lux::Event& event) override
    {

        return camera_obj.on_event(event);
    }

    void on_update() override
    {
        LGA::Begin("Test", { 0.0f, 0.0f}, 0.5, 0.5);

        LGA::End();

        LGA::Begin("Test2", { 0.0f, 0.0f}, 0.5, 0.5);

        LGA::End();
    }

};



void lux_start()
{
    Lux::Application::PushLayer<EngineLayer>();
}