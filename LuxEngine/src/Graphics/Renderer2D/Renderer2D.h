#pragma once 

#include "Utils/Types.h"

#include "Assets/Texture.h"
#include "Assets/Font.h"

#include <string_view>

namespace Lux
{

class Renderer2D
{
private:

    Renderer2D() {}

public:

    static void Init();

    static void Shutdown();

    static void BeginScene(const mat4& projection);

    static void EndScene();

    static void DrawTexturedRect(const mat4& transform, const v4& color, const Ref<Texture>& texture, float tiling = 0.0f);

    static void DrawRect(const mat4& transform, const v4& color);

    static void DrawText(std::string_view text, const mat4& transform, const v4& color, Ref<Font>& font);

};

}