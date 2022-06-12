#include "GUILayer.h"

#include "Core/Input.h"

#include "Utils/Logger.h"

#include "GUIStyle.h"

#include <algorithm>

namespace Lux::GUI
{


GUILayer* GUILayer::s_Instance = nullptr;

void GUILayer::on_update()
{

    GUI::Renderer.Get.Draw(cache);
}


bool GUILayer::on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event)
{
    (void) event;
}


bool GUILayer::on_mouse_move(const Event<EventType::MouseMoved>& event)
{
    (void) event;
}

bool GUILayer::on_resize(const Event<EventType::WindowResize>& event)
{
    // TODO: notify every box etc. about resizing changes 
    GUI::Renderer.Get().resize({event.height, event.width});
    return m_camera.on_resize(event);
}

}