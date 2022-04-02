#include "CameraController.h"

#include "Core/Input.h"
#include "Utils/Types.h"
#include "Utils/Logger.h"

namespace Lux
{
Camera2DController::Camera2DController()
{
    m_AspectRatio = Application::Width() / Application::Height();
    
    m_Camera = std::make_shared<Camera2D>(
        -m_AspectRatio * m_ZoomLevel,
         m_AspectRatio * m_ZoomLevel,
        -m_ZoomLevel,
         m_ZoomLevel
    );
}

    
bool Camera2DController::on_event(const Event& event)
{
    float scrollDistance = 0.0f;
    v2 camera_movement;
    switch(event.type())
    {
        case EventType::Scrolled:
            scrollDistance = event.delta.y;

            if(!scrollDistance)
                return false;

            
            if(scrollDistance < 0)
                m_ZoomLevel *=  (-scrollDistance) * m_ZoomIntensity;
            else 
                m_ZoomLevel /= scrollDistance * m_ZoomIntensity; 

            m_ZoomLevel = std::max(m_ZoomLevel, 0.01f);

            m_Camera->set_projection(
                -m_AspectRatio * m_ZoomLevel,
                 m_AspectRatio * m_ZoomLevel,
                -m_ZoomLevel,
                 m_ZoomLevel
            );

            return true;

        case EventType::WindowResize:

            m_AspectRatio = (float)event.width / (float)event.height;
            
            m_Camera->set_projection(
                -m_AspectRatio * m_ZoomLevel,
                 m_AspectRatio * m_ZoomLevel,
                -m_ZoomLevel,
                 m_ZoomLevel
            );

            return true;

        case EventType::MouseMoved:

            camera_movement = {
                -event.delta.x * m_Camera->width(),
                -event.delta.y * m_Camera->height()
            };

            if(Input::GetState(MouseKey::LEFT) == KeyState::Pressed)
                m_Camera->add_position(camera_movement);

            return true;

    }

    return false;
}

}