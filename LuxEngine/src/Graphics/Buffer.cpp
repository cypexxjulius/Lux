#include "Buffer.h"

#include "System/OpenGL/Buffer.h"
#include "Graphics/Renderer.h"

#include "Utils/Assert.h"

namespace Lux
{

VertexBuffer* VertexBuffer::Create(void* data, u32 size)
{
    if(Renderer::GetRendererAPI() == RendererAPI::OpenGL)
        return new OpenGL::VertexBuffer(data, size);

    TODO();

    return nullptr;
}

VertexBuffer* VertexBuffer::Create(u32 size)
{
    if(Renderer::GetRendererAPI() == RendererAPI::OpenGL)
        return new OpenGL::VertexBuffer(size);

    
    TODO();

    return nullptr;
}

IndexBuffer* IndexBuffer::Create(unsigned int* data, u32 count)
{
    if(Renderer::GetRendererAPI() == RendererAPI::OpenGL)
        return new OpenGL::IndexBuffer(data, count);

    
    TODO();

    return nullptr;
}

}