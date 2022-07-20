#include "Application.h"

void lux_start();

int main()
{
    std::cout << "Starting LuxEngine" << std::endl;
    auto* app = new Lux::Application("Start Project");

    lux_start();

    app->run();

    delete app;
}