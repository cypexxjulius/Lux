
#include "VertexArray.h"

#include "Utils/Types.h"
#include "Utils/Assert.h"


#include <glad/glad.h>

namespace Lux::OpenGL
{

static inline GLenum Lux_to_gl_type(DataType type)
{
    switch(type)
    {
        case DataType::FLOAT : return GL_FLOAT;
        case DataType::BOOL : return GL_BOOL;
        case DataType::CHAR : return GL_BYTE;
        case DataType::BYTE : return GL_BYTE;
        case DataType::INT : return GL_INT;
        default:

            TODO();
            return 0;

    }
}


VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_ID);

    for(auto& vb : m_VertexBuffers)
    {
        delete vb;
        vb = nullptr;
    }
    
    delete m_IndexBuffer;
    m_IndexBuffer = nullptr;
}

static inline void vertex_array_bind(u32 id)
{
    static i32 bound_adress = -1;
    
    if(static_cast<i32>(id) == bound_adress)
        return;
    
    glBindVertexArray(id);
    bound_adress = id;
}

void VertexArray::add_vertex_buffer(VertexBuffer* vb)
{
    bind();
    vb->bind();

    const auto& layout = vb->layout();

    m_Index = 0;
    for(const auto& element : layout)
    {   
        glEnableVertexAttribArray(m_Index);
        glVertexAttribPointer(
            m_Index,
            element.count,  
            Lux_to_gl_type(element.type), 
            static_cast<int>(element.normalized), 
            vb->stride(), 
            (const void*)(intptr_t)element.offset
        );
        
        m_Index++;
    }

    m_VertexBuffers.push_back(vb);
}


void VertexArray::bind()
{
   vertex_array_bind(m_ID);  
}

void VertexArray::set_index_buffer(IndexBuffer* ib)
{
    Verify(ib != nullptr);
    
    bind();
    ib->bind();
    m_IndexBuffer = ib;
}

VertexBuffer* VertexArray::vertexbuffer(u32 index)
{
    return m_VertexBuffers[index];
}

}