#include "Manager.h"

#include "GUI/Context.h"


namespace Lux::GUI
{

ECS::Registry* Manager::s_Registry = nullptr;
Context* Manager::s_Context = nullptr;

UUID Manager::create(TypeComponent type)
{
	UUID id = s_Context->create_gui_element();
	get_component<TypeComponent>(id) = type;
	return id;
}


}