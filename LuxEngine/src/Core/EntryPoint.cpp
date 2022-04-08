#include "Application.h"

void lux_start();

int main()
{
    Lux::Application::Start("Start Project");

    lux_start();

    Lux::Application::Run();
}