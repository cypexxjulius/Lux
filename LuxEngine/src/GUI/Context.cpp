#include "Context.h"

#include "ECS/Components.h"

#include "Assets/AssetManager.h"

namespace Lux::GUI
{


Context::Context()
{
	m_Registry.register_components<Components::Types>();

	GUIObject::Init(*this, m_Registry);
	
	auto& elements = m_Registry.view<TransformComponent, RectComponent>();

	m_Font = ResourceManager::CreateFont("GUIStandardFont", { "res/fonts/Roboto-Regular.ttf" });
}

Context::~Context()
{
	m_RootElement->shutdown();

	// Todo simple Hack for cleaning up the gui elements ;>)
	while(!m_GUIElements.empty())
	{
		m_GUIElements.begin()->second->shutdown();	
	}

	GUIObject::Shutdown();
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

	GUIObject::SetDimension(width, height);

	refresh();
}

void Context::set_root(GUIObject* root)
{
	Verify(root);
	Verify(root->get<TypeComponent>() == TypeComponent::SECTION);
	Verify(root->get<LayoutComponent>().parent == 0);

	m_RootElement = root;

	refresh();
}

void Context::remove_element(UUID id)
{
	m_GUIElements.erase(id);
}

void Context::register_element(UUID id, GUIObject* obj)
{
	Verify(!map_contains(m_GUIElements, id))

	m_GUIElements.insert({id, obj});
}


GUIObject& Context::get_object(UUID id)
{
	Verify(map_contains(m_GUIElements, id));

	return *m_GUIElements.at(id);
}

void Context::refresh()
{
	if(!m_RootElement)
		return;

	m_RootElement->refresh({ 0.0f , 0.0f}, m_Width, m_Height, 0);
}

}