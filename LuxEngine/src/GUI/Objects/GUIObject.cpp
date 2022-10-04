#include "GUIObject.h"

#include "GUI/Context.h"

namespace Lux::GUI
{
Context* GUIObject::s_Context = nullptr;
float GUIObject::s_Width = 0.0f, GUIObject::s_Height = 0.0f;

GUIObject::~GUIObject()
{
	s_Context->remove_element(m_ID);
}

GUIObject::GUIObject(GUIType type)
	:	m_ID(), 
		m_Type(type)
{
	s_Context->register_element(m_ID, this);
}

GUIObject& GUIObject::GetObject(UUID id)
{
	return s_Context->get_object(id);
}

void GUIObject::SetRoot(UUID id)
{
	s_Context->set_root(id);
}

void GUIObject::register_glyph(Transform* transform, Glyph* glyph)
{
	s_Context->add_to_render_queue(m_ID, transform, glyph);
}

void GUIObject::register_rect(Transform* transform, Rect* rect)
{
	s_Context->add_to_render_queue(m_ID, transform, rect);
}

void GUIObject::unregister_glyph()
{
	s_Context->remove_from_render_queue<Glyph>(m_ID);
}

void GUIObject::unregister_rect()
{
	s_Context->remove_from_render_queue<Rect>(m_ID);
}

const Ref<Font>& GUIObject::GetFont()
{
	return s_Context->get_font();
}

}