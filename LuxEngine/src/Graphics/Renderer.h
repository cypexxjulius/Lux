#pragma once 

#include "Utils/Types.h"
#include "VertexArray.h"

#include <functional>


namespace Lux
{

enum class RendererAPI
{
    NA,
    OpenGL,
    Vulcan,
    DirectX12,
    Metal
};

struct RendererFunctions
{
    std::function<void()> Init;
    std::function<void()> Shutdown;

    std::function<void()> Clear;
    std::function<void(const v4& color)> SetClearColor;
    std::function<void(u32 width, u32 height)> SetViewport;
    std::function<void(Ref<VertexArray> va, u32 indexCount)> DrawIndexed;
};


class Renderer
{
private:
    static RendererFunctions s_functions;
    static bool s_initialized;
    static RendererAPI m_api;

public: 

    static void Init(RendererAPI api);

    static void Shutdown();

    static inline RendererAPI GetRendererAPI()
    {
        return m_api;
    }

    static inline void Clear()
    {
        s_functions.Clear();
    }

    static inline void SetClearColor(const v4& color)
    {
        s_functions.SetClearColor(color);
    }

    static inline void DrawIndexed(Ref<VertexArray> va, u32 index_count = 0)
    {
        s_functions.DrawIndexed(va, index_count);
    }

    static void SetViewport(u32 width, u32 height)
    {
        s_functions.SetViewport(width, height);
    }

};


}