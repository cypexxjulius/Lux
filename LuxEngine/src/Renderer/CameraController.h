#pragma once 

#include <initializer_list>

#include "Camera2D.h"
#include "Event/Event.h"

namespace Lux
{

enum class Camera2DControll
{
    DRAG = 0b1,
    SCROLL = 0b10
};

class Camera2DController 
{
private:
    float m_ZoomIntensity = 2.0f;
    float m_CameraTranslationSpeed = 5.0f; 
    float m_CameraRotationSpeed = 1.0f;

    float m_AspectRatio = 1.0f;
    float m_ZoomLevel = 1.0f;

    u32 m_Controlls;

    v2 m_MouseMovedDelta;
    Camera2D* m_Camera;


public:

    Camera2DController(std::initializer_list<Camera2DControll> init_list);

    ~Camera2DController();
    
    bool OnEvent(const Event& event);

    constexpr const Camera2D& camera() const
    { return *m_Camera; }

};


}