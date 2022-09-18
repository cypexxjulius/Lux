#include "GUIObject.h"

#include "GUI/Context.h"


namespace Lux::GUI
{

ECS::Registry* GUIObject::s_Registry = nullptr;
Context* GUIObject::s_Context = nullptr;
float GUIObject::s_Width = 0.0f, GUIObject::s_Height = 0.0f;

GUIObject::~GUIObject()
{
	s_Context->remove_element(m_ID);
}

GUIObject::GUIObject(TypeComponent type)
	: m_ID(s_Registry->create())
{
	s_Context->register_element(m_ID, this);

	attach_component<TypeComponent>() = type;
}


GUIObject& GUIObject::GetObject(UUID id)
{
	return s_Context->get_object(id);
}


void GUIObject::set_root()
{
	return s_Context->set_root(this);
}

const Ref<Font>& GUIObject::GetFont()
{
	return s_Context->get_font();
}

}