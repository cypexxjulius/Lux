#include "CameraController.h"

#include "Core/Input.h"
#include "Utils/Types.h"
#include "Utils/Logger.h"

namespace Lux
{
Camera2DController::Camera2DController()
{
    m_AspectRatio = Application::AspectRatio();
    
    m_Camera = std::make_shared<Camera2D>(
        -m_AspectRatio * m_ZoomLevel,
         m_AspectRatio * m_ZoomLevel,
        -m_ZoomLevel,
         m_ZoomLevel
    );
}

    
bool Camera2DController::on_event(const Event& event)
{
    switch(event.type())
    {
        case EventType::WindowResize:

            m_AspectRatio = (float)event.width / (float)event.height;
            
            m_Camera->set_projection(
                -m_AspectRatio * m_ZoomLevel,
                 m_AspectRatio * m_ZoomLevel,
                -m_ZoomLevel,
                 m_ZoomLevel
            );

            return true;
    }

    return false;
}

}