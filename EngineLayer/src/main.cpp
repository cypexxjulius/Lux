#include <Lux.h>
#include <iostream>

class EngineLayer final : public Lux::Layer
{
private:

public:

    EngineLayer() {}

    void on_attach() override
    {
        std::cout << "Hello World" << std::endl;
    }

    void on_detach() override
    {

    }

    void on_event() override
    {


    }

    void on_update() override
    {

    }

};



void lux_start()
{
    Lux::Application::Get().push_layer<EngineLayer>();
}