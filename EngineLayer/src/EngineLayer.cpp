#include <Lux.h>
#include <iostream>

class EngineLayer final : public Lux::Layer
{
private:

public:

    EngineLayer() {}

    void on_attach() override
    {
        
    }

    void on_detach() override
    {

    }

    bool on_event(const Lux::Event& event) override
    {

        return false;
    }

    void on_update() override
    {
        std::cout << "I'am a loop hero" << std::endl;
    }

};



void lux_start()
{
    Lux::Application::Get().push_layer<EngineLayer>();
}