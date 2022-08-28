#include "Manager.h"

#include "GUI/Context.h"


namespace Lux::GUI
{

ECS::Registry* Manager::s_Registry = nullptr;
Context* Manager::s_Context = nullptr;

UUID Manager::create(TypeComponent type, const std::string& name)
{
	return s_Context->create_gui_element(type, name);
}

void Manager::refresh_section(UUID id)
{
	s_Context->refresh_section(id);
}

}