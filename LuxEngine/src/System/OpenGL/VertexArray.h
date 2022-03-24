#pragma once 

#include <vector>

#include "Graphics/VertexArray.h"

#include "Utils/Types.h"

namespace Lux::OpenGL
{

class VertexArray final : public ::Lux::VertexArray
{
private:

    u32 m_ID, m_Index = 0;
    IndexBuffer* m_IndexBuffer = nullptr; 
    std::vector<::Lux::VertexBuffer*> m_VertexBuffers;

public:

    VertexArray();

    ~VertexArray();

    virtual void add_vertex_buffer(::Lux::VertexBuffer* VertexBuffer) override;

    virtual void bind() override;

    virtual void set_index_buffer(::Lux::IndexBuffer* ib) override;

    virtual VertexBuffer* vertexbuffer(u32 index) override;

    virtual IndexBuffer* indexbuffer() override
    { return m_IndexBuffer; }

};
    

}