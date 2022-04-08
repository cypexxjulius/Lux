#include "GUILayer.h"

#include "Core/Input.h"

#include "Utils/Logger.h"

#include "GUIStyle.h"

namespace Lux::GUI
{


GUILayer* GUILayer::s_Instance = nullptr;

void GUILayer::on_update()
{
    Renderer2D::BeginScene(m_camera.camera());

    for(const auto& box : m_boxes)
    {
        Renderer2D::Draw(box.render_component);

        Renderer2D::Draw(box.text_render_component);
    }


    Renderer2D::EndScene();
}


bool GUILayer::on_event(const Event& event)
{
    switch(event.type())
    {
        case EventType::MouseButtonPressed:

            switch(event.action)
            {
                case KeyState::Pressed:
                    for(int i = static_cast<int>(m_boxes.size()) - 1; i >= 0; i--)
                    {
                        Box& box = m_boxes[i]; 
                        
                        if(box.is_intersecting(event.position))
                        {
                            box.select();
                            m_selected_box = m_dragged_box = &box;
                            return true;
                        }
                    }

                    if(m_selected_box)
                    {
                        m_selected_box->unselect();
                        m_selected_box = nullptr;
                    }

                case KeyState::Released:

                    if(m_dragged_box)
                    {
                        m_dragged_box->undrag();
                        m_dragged_box = nullptr;
                    }

            }


        case EventType::MouseMoved:

            if(m_dragged_box)
            {
                m_dragged_box->add_position_vector(event.delta);
            }
    }

    m_camera.on_event(event);
    return false;
}

}