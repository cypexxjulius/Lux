#pragma once 

#include <initializer_list>

#include "Core/Event.h"
#include "Camera2D.h"

namespace Lux
{

class Camera2DController 
{
private:
    float m_AspectRatio = 1.0f;
    float m_ZoomLevel = 1.0f;

    Camera2D m_Camera{1.0f, 1.0f, 1.0f, 1.0f};


public:

    Camera2DController();
    
    bool on_resize(const Event<EventType::WindowResize>& event);

    inline const glm::mat4& get_projection() const
    { return m_Camera.view_proj_mat(); }

};


}