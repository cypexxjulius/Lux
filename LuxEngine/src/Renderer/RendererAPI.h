#pragma once 

#include "Utils/Types.h"

#include "VertexArray.h"

namespace Lux
{


class RendererAPI
{
private:
    static RendererAPI* s_Instance; 

protected:
    RendererAPI() = default;

public:

    virtual ~RendererAPI() {} 

    virtual void init() = 0;

    virtual void shutdown() = 0;

    virtual void clear(const v4& color = { 0.8f, 0.0f, 0.0f, 1.0f } ) = 0;

    virtual void draw_indexed(VertexArray* va, u32 indexCount = 0) = 0;

    virtual void set_viewport(u32 width, u32 height) = 0;

    static RendererAPI* create();

    static constexpr RendererAPI* Get()  
    { return s_Instance; }

};


}