#include <Lux.h>
#include <iostream>
#include <memory>

using namespace Lux;

class EngineLayer final : public Lux::Layer
{
private:
    Camera3D m_Camera3D{45.0f, Application::AspectRatio()};
    v2 text_dimemsions{};
    
public:

    void on_attach() override
    {
        GUI::Section body = GUI::Section::Create("Body");
        body.make_root();

        body.set_orientation(GUI::LayoutOrientation::VERTICAL);

        GUI::Section TopBar =  GUI::Section::Create("TopBar");
        
        GUI::Section PageContent = GUI::Section::Create("PageContent");
        PageContent.scale(8);


        body.attach({TopBar, PageContent});

        GUI::Section LeftSection = GUI::Section::Create("LeftSection");


        GUI::Section RightSection = GUI::Section::Create("RightSection");


        GUI::Section MiddleSection = GUI::Section::Create("MiddleSection");
        MiddleSection.scale(3);
        MiddleSection.set_orientation(GUI::LayoutOrientation::VERTICAL);

        GUI::Section ViewPortSection = GUI::Section::Create("ViewPortSection");
        ViewPortSection.scale(4);
        ViewPortSection.make_scalable();

        GUI::Section UtilsSection = GUI::Section::Create("UtilsSection");
        UtilsSection.make_scalable();

        MiddleSection.attach({ ViewPortSection, UtilsSection });
        

        PageContent.attach({LeftSection, MiddleSection, RightSection});
    }



    void on_detach() override
    {

    }

    void on_update() override
    {
        /*
        Renderer2D::BeginScene(m_Camera3D.projection());


        u32 index = 0;
        for (auto& transform : transforms)
        {
            Renderer2D::DrawRect(transform, { 0.0f, (1 / transforms.size()) * index++, 0.0f, 1.0f });

        }



        // Renderer2D::DrawText("Hello World", text_transform, { 0.0f, 0.0f, 0.0f, 1.0f }, m_used_font);

        Renderer2D::EndScene();
        */
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