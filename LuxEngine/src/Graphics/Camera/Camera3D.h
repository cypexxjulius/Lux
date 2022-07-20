#pragma once 

#include "Utils/Types.h"
#include "Utils/Logger.h"

#include "Core/Event.h"
#include "Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>



namespace Lux
{

class Camera3D
{
public:

	static constexpr float NEAR_CLIP = 0.1f;
	static constexpr float FAR_CLIP = 1000.0f;

public:
    Camera3D(float fov, float AspectRatio);


	void on_resize(const Event<EventType::WindowResize>& event)
	{
		m_AspectRatio = event.width / event.height;
		m_ViewportHeight = event.height;
		m_ViewportWidth = event.width;

		update_projection();
		update_view();
	}

	bool on_mouse_move(const Event<EventType::MouseMoved>& event)
	{
		v2 delta = event.delta * 4.0f;
		
		if(delta.x == 0 && delta.y == 0)
			return true;


		if (Input::GetState(MouseKey::MIDDLE) == KeyState::Pressed)
			mouse_pan(delta);
		else if (Input::GetState(MouseKey::LEFT) == KeyState::Pressed)
			mouse_rotate(delta);
		else if (Input::GetState(MouseKey::RIGHT) == KeyState::Pressed)
			mouse_zoom(delta.y);

		update_view();
			
		return true;
	}

	bool on_scroll(const Event<EventType::Scrolled>& event)
	{
		mouse_zoom(event.delta.y);
		update_view();

		return true;
	}
	
	const mat4 projection() const
	{
		return m_Projection * m_ViewMatrix;
	}
    
private:

	void update_projection();

	void update_view();

	inline float get_distance() const
	{
		return m_Distance;
	}

	inline void set_distance(float distance)
	{
		m_Distance = distance;
	}

	inline void pan_speed(float* xFactor, float* yFactor) const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		*xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		*yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
	}

	inline float zoom_speed() const 
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);

		float speed = distance * distance;
		speed = std::min(speed, 100.0f);
		return speed;

	}

	inline glm::quat get_orientation() const 
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

	inline v3 get_right_orientation() const
	{
		return glm::rotate(get_orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	inline v3 get_up_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	inline v3 get_forward_direction() const
	{
		return glm::rotate(get_orientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	inline void mouse_pan(const v2& delta)
	{
		float xFactor, yFactor;
		pan_speed(&xFactor, &yFactor);

		m_FocalPoint += -get_right_orientation() * delta.x * xFactor * m_Distance;
		m_FocalPoint += get_up_direction() * delta.y * yFactor * m_Distance;
	}
	inline void mouse_rotate(const glm::vec2& delta)
	{
		float yawSign = get_up_direction().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * m_RotationSpeed;
		m_Pitch += delta.y * m_RotationSpeed;
	}
	
	inline void mouse_zoom(float delta)
	{
		m_Distance -= delta * zoom_speed();
		
		m_Distance = std::max(1.0f, m_Distance);

		INFO("{}", m_Distance);
		m_Distance = std::min(m_Distance, 100.0f);
	}

	inline v3 calculate_position() const
	{
		return m_FocalPoint - get_forward_direction() * m_Distance;
	}
private:
	glm::mat4 m_Projection{glm::mat4(1.0f)};

	float m_FOV, m_AspectRatio;

	mat4 m_ViewMatrix;
	v3 m_Position{ 0 };
	v3 m_FocalPoint{ 0 };


	float m_ViewportWidth = 1280.0f, m_ViewportHeight = 720.0f;

	float m_Distance{ 5.0f };
	float m_Pitch{ 0.0f }, m_Yaw{ 0.0f };


	float m_RotationSpeed = 0.8f;
};

}