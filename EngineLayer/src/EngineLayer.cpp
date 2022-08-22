#include <Lux.h>
#include <iostream>
#include <memory>

using namespace Lux;

class EngineLayer final : public Lux::Layer
{
private:
    
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

    }
    virtual bool on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event) override
    {
        return false;
    }

    virtual bool on_mouse_move(const Event<EventType::MouseMoved>& event) override
    {
        return false;
    }

    virtual void on_resize(const Event<EventType::WindowResize>& event) override
    {
    }

    virtual bool on_scroll(const Event<EventType::Scrolled>& event) override
    {
        return false;
    }

};



void lux_start()
{
    Lux::Application::PushLayer<EngineLayer>();
}