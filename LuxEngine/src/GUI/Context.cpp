#include "Context.h"

#include "ECS/Components.h"

#include "Managers/SectionManager.h"
#include "Interface/SectionInterface.h"

namespace Lux::GUI
{


Context::Context()
{
	m_Registry.register_component<TransformComponent, LayoutComponent, LayoutInfo, RectComponent, TextComponent, GlyphComponent>();

	Manager::Init(*this, m_Registry);

	register_manager<SectionManager>();
}

Context::~Context()
{
	for(auto& manager : m_Managers)
		manager.second->shutdown();

	Manager::Shutdown();
}

UUID Context::create_gui_element()
{
	UUID id = m_Registry.create();

	m_Registry.add_components<TransformComponent, LayoutComponent, TypeComponent>(id);
	return id;
}

void Context::set_root(UUID id)
{
	m_Registry.get_component<TypeComponent>(id);

}

}