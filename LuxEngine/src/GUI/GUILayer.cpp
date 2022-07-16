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
    Renderer2D::BeginScene(m_camera.projection());

    mat4 transform = glm::translate(mat4{ 1.0f }, { -1.0f, 0.0f, 1.0f });
     
    Renderer2D::DrawRect(transform, {0.0f, 0.0f, 0.8f, 1.0f});

    transform *= glm::scale(mat4{ 1.0f }, { 0.3, 0.3, 1.0f });

    Renderer2D::DrawText("Hello World", transform, { 0.0f, 0.0f, 0.0f, 1.0f }, m_used_font);

    Renderer2D::EndScene();
}


bool GUILayer::on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event)
{
    (void) event;
    return false;
}


bool GUILayer::on_mouse_move(const Event<EventType::MouseMoved>& event)
{
    (void) event;
    return false;
}

void GUILayer::on_resize(const Event<EventType::WindowResize>& event)
{
    // TODO: notify every box etc. about resizing changes 
    m_camera.on_resize(event);
}

}