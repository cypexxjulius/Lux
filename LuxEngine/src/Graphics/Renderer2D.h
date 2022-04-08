#pragma once 

#include "Utils/Types.h"

#include "Camera2D.h"
#include "VertexArray.h"
#include "Assets/Texture.h"

#include "Renderable2D.h"

#include <utility>

namespace Lux
{

struct Renderer2DVertex
{
    v3 Position;
    v4 Color;
    v2 TexCoord;
    float TextureSlot;
    float Tiling;
};


class Renderer2D
{
    static inline void UploadBatch();

    static void DrawRect(const Renderable2D<Renderable2DType::Rect>& rect);

    static void DrawText(const Renderable2D<Renderable2DType::Text>& rect);

public:

    static void Init();

    static void Shutdown();

    static void BeginScene(const Ref<Camera2D>& camera);

    static void EndScene();

    template<Renderable2DType Type>
    static void Draw(const Renderable2D<Type>& element)
    {
        if constexpr(Type == Renderable2DType::Rect)
            return DrawRect(element);

        if constexpr(Type == Renderable2DType::Text)
            return DrawText(element);
    }


};

}