#include "Context.h"

#include "ECS/Components.h"


#include "Managers/LayoutManager.h"
#include "Managers/SectionManager.h"
#include "Managers/TextManager.h"


#include "Interface/SectionInterface.h"


namespace Lux::GUI
{


Context::Context()
{
	m_Registry.register_components < ComponentGroup > ();

	Manager::Init(*this, m_Registry);

	register_manager<SectionManager>();
	
	register_manager<LayoutManager>();

	register_manager<TextManager>();
	
	auto& elements = m_Registry.view<TransformComponent, RectComponent>();
}

Context::~Context()
{
	for(auto& manager : m_Managers)
		manager.second->shutdown();

	Manager::Shutdown();
}

UUID Context::create_gui_element(TypeComponent type, const std::string& name)
{
	UUID id = m_Registry.create();

	m_Registry.add_component<TransformComponent>(id);
	m_Registry.add_component<TypeComponent>(id) = type;
	m_Registry.add_component<LayoutComponent>(id) = LayoutManager::Default(name);

	return id;
}

void Context::set_root(UUID id)
{
	Verify(m_Registry.get_component<TypeComponent>(id) == TypeComponent::SECTION);
	Verify(m_Registry.get_component<LayoutComponent>(id).parent == 0);

	m_RootElement = id;

	update_root();
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

void Context::render_glyphs(std::function<void(const TransformComponent&, const GlyphComponent&)> render_callback)
{
	auto& elements = m_Registry.view<TransformComponent, GlyphComponent>();

	for(auto element : elements.get_elements())
	{
		render_callback(
			m_Registry.get_component<TransformComponent>(element), 
			m_Registry.get_component<GlyphComponent>(element)	
		);
	}
}

void Context::update_dimensions(float width, float height)
{
	m_Width = width;
	m_Height = height;

	
	auto manager = get_manager<LayoutManager>();
	manager->set_dimensions(m_Width, m_Height);

	if(!m_RootElement)
		return;

	update_root();
}

void Context::force_refresh()
{
	update();		
}


void Context::refresh_section(UUID id)
{
	auto manager = get_manager<LayoutManager>();

	
	update(id);	
}

void Context::update(UUID id)
{
	auto manager = get_manager<LayoutManager>();

	if(!id)
		id = m_RootElement;

	manager->recalculate_section(id);	
	
}

void Context::update_root()
{
	auto manager = get_manager<LayoutManager>();

	manager->recalculate_root(m_RootElement);	
}

}