#include "Context.h"

#include "ECS/Components.h"

#include "Assets/AssetManager.h"

#include "Objects/LayoutObject.h"

namespace Lux::GUI
{


Context::Context()
{
	GUIObject::Init(*this);
	

	m_Font = ResourceManager::CreateFont("GUIStandardFont", { "res/fonts/Roboto-Regular.ttf" });
}

Context::~Context()
{
	delete m_RootElement;

	// Todo simple Hack for cleaning up the gui elements ;>)
	while(!m_GUIElements.empty())
	{
		delete m_GUIElements.begin()->second;	
	}

	GUIObject::Shutdown();
}


void Context::render_rects(std::function<void(const Transform&, const Rect&)> render_callback)
{
	for(auto element : m_RectComponents)
	{
		render_callback(
			*element.second.first, 
			*element.second.second	
		);
	}
}

void Context::render_glyphs(std::function<void(const Transform&, const Glyph&)> render_callback)
{
	for(auto element : m_GlyphComponents)
	{
		render_callback(
			*element.second.first,
			*element.second.second
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

void Context::set_root(UUID id)
{
	Verify(m_GUIElements.find(id) != m_GUIElements.end());



	m_RootElement = static_cast<GUIChild*>(m_GUIElements.at(id));

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