#pragma once 

#include "Utils/Types.h"

namespace Lux
{

class UniformBuffer
{
public:

    virtual ~UniformBuffer() {}

    virtual void SetData(const void* data, u32 size, u32 offset = 0) = 0;


protected:
    UniformBuffer() = default;

};

}