#pragma once 

#include <vector>

#include "Graphics/Core/VertexArray.h"

#include "Utils/Types.h"

namespace Lux::OpenGL
{

class VertexArray final : public ::Lux::VertexArray
{
private:

    u32 m_ID, m_Index = 0;
    Ref<IndexBuffer> m_IndexBuffer = nullptr; 
    std::vector<Ref<::Lux::VertexBuffer>> m_VertexBuffers;

public:

    VertexArray();

    ~VertexArray();

    virtual void add_vertex_buffer(Ref<::Lux::VertexBuffer> VertexBuffer) override;

    virtual void bind() override;

    virtual void set_index_buffer(Ref<::Lux::IndexBuffer> ib) override;

    virtual Ref<VertexBuffer> vertexbuffer(u32 index) override;

    virtual Ref<IndexBuffer> indexbuffer() override
    { return m_IndexBuffer; }

};
    

}