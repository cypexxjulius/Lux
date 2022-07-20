#include "Buffer.h"

#include "System/OpenGL/Buffer.h"
#include "Graphics/Renderer.h"

#include "Utils/Assert.h"

namespace Lux
{

VertexBuffer* VertexBuffer::Create(void* data, u32 size)
{
    return new OpenGL::VertexBuffer(data, size);
}

VertexBuffer* VertexBuffer::Create(u32 size)
{
    return new OpenGL::VertexBuffer(size);
}

IndexBuffer* IndexBuffer::Create(unsigned int* data, u32 count)
{
    return new OpenGL::IndexBuffer(data, count);
}

}