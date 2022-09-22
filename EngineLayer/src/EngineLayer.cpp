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
        GUI::VSection body("Body");
        body.remove_padding();

        GUI::HSection TopBar("TopBar");
        TopBar.enable_heading();
        
        GUI::HSection PageContent("PageContent");
        PageContent.scale(8);
        

        body.attach({ TopBar, PageContent });

        GUI::VSection LeftSection("LeftSection");


        GUI::VSection RightSection("RightSection");


        GUI::VSection MiddleSection("MiddleSection");
        MiddleSection.scale(3);
        
        GUI::VSection ViewPortSection("ViewPortSection");
        ViewPortSection.scale(4);
        ViewPortSection.make_scalable();

        GUI::VSection UtilsSection("UtilsSection");
        UtilsSection.make_scalable();

        GUI::TextWidget Text("abcdefghijklmnopqrstuvwxyz");


        GUI::TextWidget Text2("Hello World this is not a thrill");
        UtilsSection.attach({ Text, Text2 });

        MiddleSection.attach({ ViewPortSection, UtilsSection });
        

        PageContent.attach({LeftSection, MiddleSection, RightSection});
        
        body.make_root();

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