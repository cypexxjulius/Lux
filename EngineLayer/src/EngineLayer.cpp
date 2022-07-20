#include <Lux.h>
#include <iostream>
#include <memory>

using namespace Lux;

class EngineLayer final : public Lux::Layer
{
private:
    Camera3D m_Camera3D{45.0f, Application::AspectRatio()};
    v2 text_dimemsions{};

    std::array<mat4, 5000> transforms;
    //GUI::BoxHandle m_boxHandle;

public:

    void on_attach() override
    {
        u32 index = 0;

        float width = 1.0f, height = 1.0f;
        for(auto& transform : transforms)
            transform = glm::translate(mat4{ 1.0f }, { -1.0f, 0.0f, (index++) / 100.0f }) * glm::scale(mat4{1.0f}, {width, height, 1.0f});

    }

    void on_detach() override
    {

    }

    void on_update() override
    {
        Renderer2D::BeginScene(m_Camera3D.projection());


        u32 index = 0;
        for (auto& transform : transforms)
        {
            Renderer2D::DrawRect(transform, { 0.0f, (1 / transforms.size()) * index++, 0.0f, 1.0f });

        }



        // Renderer2D::DrawText("Hello World", text_transform, { 0.0f, 0.0f, 0.0f, 1.0f }, m_used_font);

        Renderer2D::EndScene();
    }
    virtual bool on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event) override
    {
        return false;
    }

    virtual bool on_mouse_move(const Event<EventType::MouseMoved>& event) override
    {
        return m_Camera3D.on_mouse_move(event);
    }

    virtual void on_resize(const Event<EventType::WindowResize>& event) override
    {
        return m_Camera3D.on_resize(event);
    }

    virtual bool on_scroll(const Event<EventType::Scrolled>& event) override
    {
        return m_Camera3D.on_scroll(event);
    }

};



void lux_start()
{
    Lux::Application::PushLayer<EngineLayer>();
}