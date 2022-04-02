#pragma once 

#include <initializer_list>

#include "Core/Event.h"
#include "Camera2D.h"

namespace Lux
{

class Camera2DController 
{
private:
    float m_ZoomIntensity = 2.0f;
    float m_CameraTranslationSpeed = 5.0f; 
    float m_CameraRotationSpeed = 1.0f;

    float m_AspectRatio = 1.0f;
    float m_ZoomLevel = 1.0f;

    float m_width, m_height;

    v2 m_camera_aspect{};

    Ref<Camera2D> m_Camera = nullptr;


public:

    Camera2DController();
    
    bool on_event(const Event& event);

    inline Ref<Camera2D> camera() const
    { return m_Camera; }

};


}