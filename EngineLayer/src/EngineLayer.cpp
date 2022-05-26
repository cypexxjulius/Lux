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

    void on_update() override
    {
        LGA::Begin("Test ggjj", { 0.0f, 0.0f}, 0.3f, 0.2f);

        LGA::End();

        LGA::Begin("Test2", { 0.0f, 0.0f}, 0.3f, 0.2f);

        LGA::End();
    }

};



void lux_start()
{
    Lux::Application::PushLayer<EngineLayer>();
}