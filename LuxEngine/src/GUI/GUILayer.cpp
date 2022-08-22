#include "GUILayer.h"

namespace Lux::GUI
{
GUILayer* GUILayer::s_Instance = nullptr;

void GUILayer::on_attach()
{
	Verify(!s_Instance);
	s_Instance = this;
}
void GUILayer::on_detach()
{
	s_Instance = nullptr;
}
bool GUILayer::on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event)
{
	return false;
}
bool GUILayer::on_key_press(const Event<EventType::KeyPressed>& event)
{
	return false;
}
bool GUILayer::on_mouse_move(const Event<EventType::MouseMoved>& event)
{
	return false;
}
void GUILayer::on_resize(const Event<EventType::WindowResize>& event)
{
}
bool GUILayer::on_scroll(const Event<EventType::Scrolled>& event)
{
	return false;
}
void GUILayer::on_update()
{
}

void GUILayer::set_root(UUID id)
{
	s_Instance->m_Context.set_root(id);
}
}