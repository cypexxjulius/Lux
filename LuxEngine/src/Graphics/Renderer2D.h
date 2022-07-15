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

public:

    static void Init();

    static void Shutdown();

    static void BeginScene(const Ref<Camera2D>& camera);

    static void EndScene();

    static void DrawRect(const mat4& transform, const v4& color, Ref<Texture> texture = nullptr, float tiling = 1.0f);

    static void DrawText(std::string_view text, const v3& position, const v4& color, const Ref<Font>& font, float scale);

};

}