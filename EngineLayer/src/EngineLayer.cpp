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

        Renderer2D::BeginScene(camera_obj.camera());

        
        Renderer2D::DrawRect({
            .width = 1.0f,
            .height = 1.0,
            .position = { 0.0f, 0.0f },
            .color = { 1.0f, 0.3f, 1.0f, 1.0f},
        });

        

        

        


        Renderer2D::DrawRect({
            .width = 0.2f,
            .height = 0.2f,
            .position = { 0.0f, 0.0f },
            .color = { 0.0f, 0.0f, 1.0f, 1.0f},
        });

        Renderer2D::DrawRect({
            .width = 0.1f,
            .height = 0.1f,
            .position = { 0.0f, 0.0f },
            .color = { 0.3f, 0.0f, 1.0f, 1.0f},
        });

        Renderer2D::DrawRect({
            .width = text_dimemsions.x,
            .height = text_dimemsions.y,
            .position = { 0.0f, 0.0f },
            .color = { 0.0f, 0.0f, 0.0f, 1.0f},
        });

        const auto& [width, height] = Renderer2D::DrawText("This is a Test jql", 0.1f, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

        text_dimemsions = { width, height };

        Renderer2D::EndScene();

    }

};



void lux_start()
{
    Lux::Application::PushLayer<EngineLayer>();
}