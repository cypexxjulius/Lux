#include "GUILayer.h"

#include "Renderer2D.h"


namespace Lux::GUI
{

GUILayer* GUILayer::s_Instance = nullptr;

void GUILayer::on_update()
{
    Renderer2D::BeginScene(m_camera.camera());

    for(const auto& box : m_boxes)
    {
        Renderer2D::DrawRect({
            .width = box.width,
            .height = box.height,
            .position = box.position,
            .color = { 0.0f, 0.0f, 0.0f, 1.0f }
        });
    }


    Renderer2D::EndScene();
}

bool GUILayer::on_event(const Event& event)
{



    m_camera.on_event(event);
    return false;
}

}