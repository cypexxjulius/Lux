#include "Application.h"

void lux_start();

int main()
{
    std::cout << "Starting LuxEngine" << std::endl;
    Lux::Application::Start("Start Project");

    lux_start();

    Lux::Application::Run();
}