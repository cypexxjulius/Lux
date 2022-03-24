#pragma once 

#include <vector>

#include "Graphics/Buffer.h"

#include "Utils/Types.h"



namespace Lux::OpenGL
{

class VertexBuffer final : public ::Lux::VertexBuffer 
{
private:
    u32 m_ID, m_Stride, m_Offset;
    std::vector<VertexBufferElement> m_Elements;

public:

    VertexBuffer(void* data, u32 size);

    VertexBuffer(u32 size);

    ~VertexBuffer();

    virtual void bind() const override;

    virtual void set_data(void *data, u32 size) override;

    virtual void set_layout(std::initializer_list<VertexBufferElement> initlist) override;

    virtual const std::vector<VertexBufferElement>& layout() override
    { return m_Elements; }

    virtual inline u32 stride() const override 
    { return m_Stride; }

    virtual inline u32 offset() const override 
    { return m_Offset; }

};

class IndexBuffer final : public ::Lux::IndexBuffer
{
private:
    u32 m_ID, m_Count;

public:

    IndexBuffer(unsigned int* data, u32 count);

    ~IndexBuffer();

    virtual u32 count() const override 
    { return m_Count; }


    virtual void bind() const override;

};

}
