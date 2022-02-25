#include "Buffer.h"

#include "System/OpenGL/Buffer.h"
#include "System/SystemController.h"

#include "Utils/Assert.h"

namespace Lux
{

VertexBuffer* VertexBuffer::create(void* data, u32 size)
{
    if(SystemController::Renderer() == SystemRenderer::OpenGL)
        return new OpenGL::VertexBuffer(data, size);

    TODO();

    return nullptr;
}

VertexBuffer* VertexBuffer::create(u32 size)
{
    if(SystemController::Renderer() == SystemRenderer::OpenGL)
        return new OpenGL::VertexBuffer(size);

    
    TODO();

    return nullptr;
}

IndexBuffer* IndexBuffer::create(unsigned int* data, u32 count)
{
    if(SystemController::Renderer() == SystemRenderer::OpenGL)
        return new OpenGL::IndexBuffer(data, count);

    
    TODO();

    return nullptr;
}

}