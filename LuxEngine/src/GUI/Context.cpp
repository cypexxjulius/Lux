#include "Context.h"

#include "ECS/Components.h"

#include "Managers/SectionManager.h"
#include "Interface/SectionInterface.h"

namespace Lux::GUI
{


Context::Context()
{
	m_Registry.register_components < ComponentGroup > ();

	Manager::Init(*this, m_Registry);

	register_manager<SectionManager>();
	
	auto& elements = m_Registry.view<TransformComponent, RectComponent>();
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

	m_Registry.add_components<TransformComponent, TypeComponent>(id);
	return id;
}

void Context::set_root(UUID id)
{
	Verify(m_Registry.get_component<TypeComponent>(id) == TypeComponent::SECTION);
	Verify(m_Registry.get_component<LayoutComponent>(id).parent == 0);

	m_RootElement = id;

	update();
}

void Context::render_rects(std::function<void(const TransformComponent&, const RectComponent&)> render_callback)
{
	auto& elements = m_Registry.view<TransformComponent, RectComponent>();

	for(auto element : elements.get_elements())
	{
		render_callback(
			m_Registry.get_component<TransformComponent>(element), 
			m_Registry.get_component<RectComponent>(element)	
		);
	}
}

void Context::update()
{
	auto manager = get_manager<SectionManager>();

	if(!m_RootElement)
		return;

	manager->set_dimensions(m_Width, m_Height);
	manager->recalculate_dimensions( m_RootElement, { 0.0f, 0.0f }, m_Width, m_Height, 0);
	
}


void Context::update_dimensions(float width, float height)
{
	m_Width = width;
	m_Height = height;

	update();

	auto manager = get_manager<SectionManager>();

	if(!m_RootElement)
		return;

	manager->set_dimensions(m_Width, m_Height);

}

}