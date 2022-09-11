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
        auto body = GUI::VSection::Create("Body");
        body.remove_padding();

        auto TopBar =  GUI::HSection::Create("TopBar");
        TopBar.enable_heading();
        
        auto PageContent = GUI::HSection::Create("PageContent");
        PageContent.scale(8);
        

        body.attach({ TopBar, PageContent });

        auto LeftSection = GUI::VSection::Create("LeftSection");


        auto RightSection = GUI::VSection::Create("RightSection");


        auto MiddleSection = GUI::VSection::Create("MiddleSection");
        MiddleSection.scale(3);
        
        auto ViewPortSection = GUI::VSection::Create("ViewPortSection");
        ViewPortSection.scale(4);
        ViewPortSection.make_scalable();

        auto UtilsSection = GUI::VSection::Create("UtilsSection");
        UtilsSection.make_scalable();

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