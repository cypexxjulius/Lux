#include "Camera3D.h"



namespace Lux
{

Camera3D::Camera3D(float fov, float aspect_ratio)
	:	m_FOV(fov),
		m_AspectRatio(aspect_ratio),
		m_Projection(glm::perspective(glm::radians(fov), aspect_ratio, NEAR_CLIP, FAR_CLIP))
{
	update_view();
}

void Camera3D::update_projection()
{
	m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, NEAR_CLIP, FAR_CLIP);
}

void Camera3D::update_view()
{
	m_Position = calculate_position();

	glm::quat orientation = get_orientation();
	m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
	m_ViewMatrix = glm::inverse(m_ViewMatrix);
}



}