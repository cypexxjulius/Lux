#pragma once 

#include "Utils/Types.h"
#include "Utils/Assert.h"

#include "Graphics/Core/VertexArray.h"

namespace Lux
{

enum class RenderingOption
{
    BLEND,
    DEPTH_TEST,
    MULTISAMPLE
};

enum class BlendFunc
{
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA
};

class RendererAPI
{
public:

    virtual void init() = 0;

    virtual void shutdown() = 0;

    virtual void clear() const = 0;

    virtual void draw_indexed(const Ref<VertexArray>& va, u32 index_count) const = 0;

    virtual void set_viewport(u32 width, u32 height) const = 0;

    virtual void enable(RenderingOption option) const = 0;

    virtual void disable(RenderingOption option) const = 0;

    virtual void set_blend_function(BlendFunc src,BlendFunc function) const = 0;

    virtual void set_clear_color(const v4& color) const = 0;

    virtual void enable_debug() const = 0;

    virtual void disable_debug() const = 0;

};

class Renderer
{
private:
    static bool s_initialized;
    static Scope<RendererAPI> m_API;

public: 
    static void Init();

    static void Shutdown();

    static inline const RendererAPI& get_api()
    {
        Verify(m_API);
        return *m_API;
    }
};




}