#include "Camera2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Lux
{
Camera2D::Camera2D(float left, float right, float top, float bottom)
    : m_ProjMat(glm::ortho(left, right, bottom, top, NEAR_VAL, FAR_VAL))
    , m_ViewMat(1)
    , m_Position({0.0f, 0.0f})
    , m_Rotation(0.0f)
{
    m_Width = abs(left) + abs(right);
    m_Height = abs(top) + abs(bottom);
    recalculate();
}

void Camera2D::recalculate()
{
    mat4 transform = glm::translate(glm::mat4(1.0f), { m_Position.x, m_Position.y, 1.0f }) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		
	m_ViewMat = glm::inverse(transform);
	m_ViewProjMat = m_ProjMat * m_ViewMat;
}

void Camera2D::set_projection(float left, float right, float top, float bottom)
{
    m_ProjMat = glm::ortho(left, right, bottom, top, NEAR_VAL, FAR_VAL);
    recalculate();

    m_Width = abs(left) + abs(right);
    m_Height = abs(top) + abs(bottom);
}

}