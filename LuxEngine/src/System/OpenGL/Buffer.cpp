
#include "Buffer.h"
#include <glad/glad.h>

#include "Utils/Types.h"
#include "Utils/Assert.h"

namespace Lux::OpenGL
{

static int convert_Lux_to_gl_type(DataType type)
{
    switch(type)
    {
        case DataType::FLOAT :  return sizeof(GLfloat);
        case DataType::INT:    return sizeof(GLint);
        case DataType::BOOL:   return sizeof(GLboolean);
        case DataType::CHAR:   return sizeof(GLbyte);
        default: 
            TODO(); // "Invalid type"
    }
    return 0;
}

    
VertexBuffer::VertexBuffer(void* data, u32 size)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(u32 size)
{

    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);

}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_ID);
}

static inline void vertex_buffer_bind(u32 id)
{
    static u32 bound_id = 0;

    if(id == bound_id)
        return;

    glBindBuffer(GL_ARRAY_BUFFER, id);
    bound_id = id;
    
}

void VertexBuffer::bind() const
{
    vertex_buffer_bind(m_ID);
}

void VertexBuffer::set_data(void *data, u32 size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::set_layout(std::initializer_list<VertexBufferElement> initlist)
{
    m_Elements = initlist;
    m_Stride = 0;
    m_Offset = 0;

    for(auto& element : m_Elements)
    {
        int size = convert_Lux_to_gl_type(element.type) * element.count; // Size of the Layout

        element.offset = m_Offset;
        m_Offset += size;
        m_Stride += size; 
        
    }
}

IndexBuffer::IndexBuffer(unsigned int* data, u32 size)
    : m_Count(size)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(u32), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_ID);
}


static inline void index_buffer_bind(u32 id)
{
    static u32 bound_id = 0;

    if(id == bound_id)
        return;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    bound_id = id;
}

void IndexBuffer::bind() const 
{
    index_buffer_bind(m_ID);
}

}