#pragma once 

#include "Utils/Types.h"

#include "Graphics/Core/Buffer.h"


namespace Lux
{

class VertexArray
{
protected:
    
    VertexArray() = default;

public:

    virtual ~VertexArray() {}

    virtual void bind() = 0;

    virtual void set_index_buffer(Ref<IndexBuffer> ib) = 0;
    
    virtual void add_vertex_buffer(Ref<VertexBuffer> vb)  = 0;

    virtual Ref<VertexBuffer> vertexbuffer(u32 index) = 0;

    virtual Ref<IndexBuffer> indexbuffer() = 0;

    static Ref<VertexArray> Create();

};

}