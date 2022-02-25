#include "CameraController.h"

#include "Core/Application.h"
#include "Utils/Types.h"

namespace Lux
{
Camera2DController::Camera2DController(std::initializer_list<Camera2DControll> init_list)
{
    m_AspectRatio = (float)Application::Get()->state().width / (float)Application::Get()->state().height;
    
    m_Camera = new Camera2D(
        -m_AspectRatio * m_ZoomLevel,
         m_AspectRatio * m_ZoomLevel,
        -m_ZoomLevel,
         m_ZoomLevel
    );


    for(const Camera2DControll& element : init_list)
        m_Controlls |= static_cast<u32>(element);

}

Camera2DController::~Camera2DController() 
{
    delete m_Camera;
    m_Camera = nullptr;
}
    
bool Camera2DController::OnEvent(const Event& event)
{
    switch(event.type())
    {
        float scrollDistance;
        case EventType::Scrolled:
            scrollDistance = event.position.y;

            if(!((m_Controlls & static_cast<u32>(Camera2DControll::SCROLL)) * scrollDistance))
                return false;
            
            if(scrollDistance < 0)
                m_ZoomLevel *=  (-scrollDistance) * m_ZoomIntensity;
            else 
                m_ZoomLevel /= scrollDistance * m_ZoomIntensity; 

            m_ZoomLevel = std::max(m_ZoomLevel, 0.0001f);

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

            if(!((m_Controlls & static_cast<u32>(Camera2DControll::DRAG)) * Application::Get()->state().mouse.buttons[0]))
                return false;
            
            m_Camera->add_position({
                -event.delta.x * (m_Camera->width() / Application::Get()->state().width), 
                -event.delta.y * (m_Camera->height() / Application::Get()->state().height), 
                0.0f
            });

            return true;

    }

    return false;
}

}