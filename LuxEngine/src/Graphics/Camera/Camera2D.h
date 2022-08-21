#pragma once

#include "Utils/Types.h"

#include "Core/Event.h"

#include "Camera.h"

namespace Lux
{

class Camera2D : public Camera
{
public:

    Camera2D(float aspect_ratio);

    void set_projection(float left, float right, float top, float bottom);

 
    inline void set_position(const v2& position)
    { m_Position = position; }

    inline void add_position(const v2& position)
    { 
        m_Position += position; 
        recalculate();
    }  

    inline v2 position() const 
    { return m_Position; }
 

    inline float rotation() const 
    { return m_Rotation; }

    inline void set_rotation(float rotation)
    { 
        m_Rotation = rotation; 
        recalculate();
    }

    virtual mat4 projection() override
    { return m_ProjMat * m_ViewMat; }

    inline float height() const
    { return m_Height; }

    inline float width() const 
    { return m_Width; }

    inline void on_resize(const Event<EventType::WindowResize>& event)
    {
        float aspect_ratio = (float)event.width / (float)event.height;

        set_projection(
            -aspect_ratio,
            aspect_ratio,
            -1.0f,
            1.0f
        );
    }

private:

    void recalculate();

private:
    static constexpr float NEAR_VAL = -10.0f;
    static constexpr float FAR_VAL = 100.0f;

    float m_Rotation;
    float m_Width, m_Height;
    v2 m_Position;
    mat4 m_ProjMat, m_ViewMat;
};

}