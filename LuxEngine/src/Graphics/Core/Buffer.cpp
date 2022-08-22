#include "Buffer.h"

#include "System/OpenGL/Buffer.h"
#include "Graphics/Renderer.h"

#include "Utils/Assert.h"

namespace Lux
{

Ref<VertexBuffer> VertexBuffer::Create(void* data, u32 size)
{
    return  create_ref<OpenGL::VertexBuffer>(data, size);
}

Ref<VertexBuffer> VertexBuffer::Create(u32 size)
{
    return create_ref<OpenGL::VertexBuffer>(size);
}

Ref<IndexBuffer> IndexBuffer::Create(unsigned int* data, u32 count)
{
    return create_ref<OpenGL::IndexBuffer>(data, count);
}

}