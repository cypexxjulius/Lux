#include "GUILayer.h"

#include "Core/Input.h"

#include "Utils/Logger.h"

#include "GUIStyle.h"

#include "Graphics/Renderer2D/Renderer2D.h"

#include <algorithm>

namespace Lux::GUI
{


GUILayer* GUILayer::s_Instance = nullptr;

void GUILayer::on_update()
{
    Renderer2D::BeginScene(m_Camera.projection());

    auto transform = glm::translate(mat4{ 1.0f }, { -1.0f, -1.0f, 0.0f }) * glm::scale(mat4{1.0f}, {0.5f, 0.5f, 1.0f});

    Renderer2D::DrawText("Hello World", transform, { 1.0f, 1.0f, 1.0f, 1.0f }, m_UsedFont);

    Renderer2D::EndScene();
}


bool GUILayer::on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event)
{
    if (event.key != MouseKey::LEFT)
        return false;

    if (event.state != KeyState::Pressed)
        return false;

    m_pressed = !m_pressed;
}


bool GUILayer::on_mouse_move(const Event<EventType::MouseMoved>& event)
{
    return false;
}

bool GUILayer::on_scroll(const Event<EventType::Scrolled>& event)
{
    m_pressed = true;
    return true;
}


void GUILayer::on_resize(const Event<EventType::WindowResize>& event)
{
    // TODO: notify every box etc. about resizing changes 
    m_Camera.on_resize(event);
    m_Camera3D.on_resize(event);
}

}