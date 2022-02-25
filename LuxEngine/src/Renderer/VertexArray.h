#pragma once 

#include "Utils/Types.h"

#include "Renderer/Buffer.h"


namespace Lux
{

class VertexArray
{
protected:
    
    VertexArray() = default;

public:

    virtual ~VertexArray() {}

    virtual void bind() = 0;

    virtual void set_index_buffer(IndexBuffer* ib) = 0;
    
    virtual void add_vertex_buffer(VertexBuffer* vb)  = 0;

    virtual VertexBuffer* vertexbuffer(u32 index) = 0;

    virtual IndexBuffer* indexbuffer() = 0;

    static VertexArray* create();

};

}