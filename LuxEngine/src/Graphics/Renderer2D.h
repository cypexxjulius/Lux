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

    static void PushTransform(mat4 transform, bool transform_override = false);

    static void PopTransform();

    static inline void PushOrigin(v2 origin)
    {
        Renderer2D::PushTransform(glm::translate(glm::mat4(1.0f), { origin.x, origin.y, 0.0f }));
    }

    static inline void PushScale(v2 scale)
    {
        Renderer2D::PushTransform(glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f }));
    }

    template<Renderable2DType Type>
    static void Draw(const Renderable2D<Type>& element)
    {
        if constexpr(Type == Renderable2DType::Rect)
            return DrawRect(element);

        if constexpr(Type == Renderable2DType::Text)
            return DrawText(element);
    }

    template<Renderable2DType Type>
    static std::pair<float, float> Dimensions(const Renderable2D<Type>& element)
    {
        return { element.m_width, element.m_height };
    }

};

}