#pragma once

#include "Utils/Types.h"

namespace Lux
{

class Camera
{
protected:

	Camera() = default;

public:

	virtual ~Camera() = default;

	virtual mat4 projection() = 0;
};

}