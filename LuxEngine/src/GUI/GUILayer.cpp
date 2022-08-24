#include "GUILayer.h"

#include "Graphics/Renderer2D/Renderer2D.h"

#include "Space.h"

namespace Lux::GUI
{
GUILayer* GUILayer::s_Instance = nullptr;

void GUILayer::on_attach()
{
	Verify(!s_Instance);
	s_Instance = this;

    m_Context.update_dimensions(m_Width, m_Height);
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
	if (event.action != KeyState::Pressed)
        return false;


    if (event.key == Key::W)
    {
        INFO("[GUILayer] switched to Camera3D");
        m_MainCamera = &m_Camera3D;
    }
    else if (event.key == Key::A)
    {
        m_MainCamera = &m_Camera;
        INFO("[GUILayer] switched to Camera2D");
    }
    else
        return false;

    return true;
}
bool GUILayer::on_mouse_move(const Event<EventType::MouseMoved>& event)
{
    return m_Camera3D.on_mouse_move(event);
}

void GUILayer::on_resize(const Event<EventType::WindowResize>& event)
{
    m_Width = event.width;
    m_Height = event.height;
    m_Context.update_dimensions(m_Width, m_Height);
}
bool GUILayer::on_scroll(const Event<EventType::Scrolled>& event)
{
    return m_Camera3D.on_scroll(event);
}
void GUILayer::on_update()
{
	Renderer2D::BeginScene(m_MainCamera->projection());

    float AspectRatio = m_Width / m_Height;

	m_Context.render_rects([&](const TransformComponent& transform, const RectComponent& rect){

        v3 scale = GUISpace::ToRenderSpaceDelta(AspectRatio, transform.scale);
	    v3 position = GUISpace::ToRenderSpace(AspectRatio, transform.position);
		

		mat4 rect_transform = glm::translate(mat4{ 1.0f }, position) *
                    glm::toMat4(glm::quat(transform.rotation)) *
                    glm::scale(mat4{ 1.0f }, scale); 

		Renderer2D::DrawTexturedRect(rect_transform, rect.color, rect.texture, rect.tiling);
	});

	Renderer2D::EndScene();
}

void GUILayer::set_root(UUID id)
{
	s_Instance->m_Context.set_root(id);
}
}