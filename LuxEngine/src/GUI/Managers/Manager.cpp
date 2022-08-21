#include "Manager.h"

#include "GUI/Context.h"


namespace Lux::GUI
{

ECS::Registry* Manager::s_Registry = nullptr;
Context* Manager::s_Context = nullptr;

UUID Manager::create()
{
	return s_Context->create_gui_element();
}


}