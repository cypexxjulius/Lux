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

    std::erase_if(m_box_positions, [](const auto box) {
        return box->is_valid == false; 
    });

    std::erase_if(m_boxes, [](const auto& box){
        return box.second.is_valid == false;
    });


    Renderer2D::BeginScene(m_camera.camera());

    for(auto rit = m_box_positions.rbegin(); rit != m_box_positions.rend(); rit++)
    {
        Renderer2D::Draw((*rit)->render_component);

        Renderer2D::Draw((*rit)->text_render_component);
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
                    for(int i = 0; i < static_cast<int>(m_box_positions.size()); i++)
                    {
                        Box& box = *m_box_positions[i]; 
                        
                        if(box.is_intersecting(event.position))
                        {
                            if(m_dragged_box)
                                m_dragged_box->undrag();

                            if(m_selected_box)
                                m_selected_box->unselect();
                            
                            m_dragged_box = m_selected_box = &box;
                            
                            m_box_positions[0] = std::exchange(m_box_positions[i], m_box_positions[0]);

                            box.select();
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