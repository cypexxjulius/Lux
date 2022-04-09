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
        (*rit)->draw();

    Renderer2D::EndScene();

    for(auto& box : m_boxes)
    {
        box.second.is_valid = false;
    }
}


bool GUILayer::on_event(const Event& event)
{
    switch(event.type())
    {
        case EventType::MouseButtonPressed:

            switch(event.action)
            {
                case KeyState::Pressed:

                    for(u32 i = 0; i < m_box_positions.size(); i++)
                    {    
                        auto* box = m_box_positions[i];
                        
                        if(box->is_intersecting(event.position))
                        {
                            m_FocusedBox.select(box, event.position);   
                            m_box_positions[0] = std::exchange(m_box_positions[i], m_box_positions[0]);
                            return true;
                        }
                    }

                    if(!m_FocusedBox.is_selected())
                        return false;

                    m_FocusedBox.unselect();
                    return true;
                    
                case KeyState::Released:

                    if(!m_FocusedBox.is_holding())
                        return false;
                    
                    m_FocusedBox.unhold();
                    return true;
                    
                default:
                    return false;

            }


        case EventType::MouseMoved:

            if(m_FocusedBox.is_holding())
            {
                m_FocusedBox.box().on_mousedelta(event.delta);
                return true;
            }

            for(auto& box : m_box_positions)
            {
                if(box->is_intersecting(event.position))
                {
                    Input::SetCursorType(box->on_hover(event.position));
                    return false;
                }
            }

            Input::SetCursorType(CursorType::Arrow);
        
        default:
            break;
    }


    m_camera.on_event(event);
    return false;
}

}