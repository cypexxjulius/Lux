#pragma once

#include "Utils/Types.h"

namespace Lux
{

class Camera2D
{
private:
    static constexpr float NEAR_VAL = 100.0f;
    static constexpr float FAR_VAL = -100.0f;

    float m_Rotation; 
    float m_Width, m_Height;
    v2 m_Position;
    mat4 m_ProjMat, m_ViewMat;
    mat4 m_ViewProjMat;

private:

    void recalculate();

public:

    Camera2D(float left, float right, float top, float bottom);

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

    inline const mat4& proj_mat() const
    { return m_ProjMat; }

    inline const mat4& view_mat() const
    { return m_ViewMat; }

    inline const mat4& view_proj_mat() const 
    { return m_ViewProjMat; } 

    inline float height() const
    { return m_Height; }

    inline float width() const 
    { return m_Width; }
    

};

}