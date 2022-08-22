#pragma once 

#include "Utils/Types.h"

#include <vector>
#include <initializer_list>



namespace Lux
{


struct VertexBufferElement
{
    DataType type;
    u32 count;
    u32 offset;
    bool normalized;

    VertexBufferElement(const char *name, DataType data_type, u32 element_count, bool is_normalized = false)
    {
        (void) name;

        type = data_type;
        count = element_count;
        offset = 0;
        normalized = is_normalized;
    }
};

class VertexBuffer
{
public:
    static constexpr int MAX_ELEMENTS = 16;

protected:
    VertexBuffer() = default;

public:

    virtual ~VertexBuffer() {}

    virtual void bind() const = 0;

    virtual void set_data(void *data, u32 size) = 0;

    virtual void set_layout(std::initializer_list<VertexBufferElement> initlist) = 0;

    virtual const std::vector<VertexBufferElement>& layout() = 0;

    virtual inline u32 stride() const = 0;
    
    virtual inline u32 offset() const = 0;
 
    static Ref<VertexBuffer> Create(void* data, u32 size);

    static Ref<VertexBuffer> Create(u32 size);
    

};

class IndexBuffer 
{
protected:
    IndexBuffer() = default;

public:

    virtual ~IndexBuffer() {}

    virtual void bind() const = 0;

    virtual u32 count() const = 0;

    static Ref<IndexBuffer> Create(unsigned int* data, u32 count);


};


}