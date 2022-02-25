#include "Renderer/RendererAPI.h"


namespace Lux::OpenGL
{

class RendererAPI final : public ::Lux::RendererAPI
{
public:
    RendererAPI();

    ~RendererAPI();

    
    virtual void init() override;

    virtual void shutdown() override;

    virtual void clear(const v4& color = { 0.8f, 0.0f, 0.0f, 1.0f } ) override;

    virtual void draw_indexed(VertexArray* va, u32 indexCount = 0) override;

    virtual void set_viewport(u32 width, u32 height) override;

    virtual double get_delta_time() override;

};


}