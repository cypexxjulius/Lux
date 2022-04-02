#include "Application.h"

void lux_start();

int main()
{
    auto* app = new Lux::Application("Start Project");

    lux_start();

    app->loop();

    delete app;
}