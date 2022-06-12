#pragma once 


namespace Lux
{

class Camera3D
{
public:


    Camera3D() = 0;

    

	const glm::mat4& get_projection() const { return m_Projection; }

private:
	glm::mat4 m_Projection{glm::mat4(1.0f)};


};

}