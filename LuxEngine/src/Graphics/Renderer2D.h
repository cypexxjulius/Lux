#pragma once 

#include "Utils/Types.h"

#include "Camera2D.h"
#include "VertexArray.h"
#include "Assets/Texture.h"

#include <utility>

namespace Lux
{

struct Rect2D
{
    float width;
    float height;
    float tiling;
    float rotation;
    Ref<Texture> texture;
    v2 position;
    v4 color;
};

class Renderer2D
{
    static void UploadBatch();

public:

    static void Init();

    static void Shutdown();

    static void BeginScene(const Ref<Camera2D>& camera);

    static void DrawRect(Rect2D&& quad);

    static std::pair<f32, f32> DrawText(std::string_view string, float scale, v2 position, v3 color);

    static void EndScene();

};

}