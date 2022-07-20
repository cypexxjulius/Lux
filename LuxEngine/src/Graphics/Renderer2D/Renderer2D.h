#pragma once 

#include "Utils/Types.h"

#include "Assets/Texture.h"
#include "Assets/Font.h"

#include "Graphics/Core/Bitmap.h"

#include <string_view>

namespace Lux
{

class Renderer2D
{
private:

    friend class Application;

    Renderer2D() {}

    static void Init();

    static void Shutdown();

public:

    static void BeginScene(const mat4& projection);

    static void EndScene();

    static void DrawTexturedRect(const mat4& transform, const v4& color, const Ref<Texture>& texture, float tiling = 0.0f);

    static void DrawBitmap(const mat4& transform, const v4& color, const Ref<Bitmap>& texture, const std::array<v2, 4>& tex_coords);

    static void DrawRect(const mat4& transform, const v4& color);

    static void DrawText(std::string_view text, const mat4& transform, const v4& color, Ref<Font>& font);

};

}