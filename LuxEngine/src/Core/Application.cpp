#include "Application.h"

namespace Lux
{

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& title)
    : m_title(title)
{
    s_Instance = this;
}


}