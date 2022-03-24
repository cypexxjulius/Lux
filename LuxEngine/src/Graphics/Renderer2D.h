#pragma once 

#include "Utils/Types.h"

#include "Shader.h"
#include "Camera2D.h"
#include "Assets/Font.h"
#include "Assets/Texture2D.h"
#include "VertexArray.h"

namespace Lux
{

struct Rect2D
{
    float width;
    float height;
    float tiling;
    float zIndex;
    float rotation;
    Ref<Texture2D> texture;
    v2 position;
    v4 color;
};

struct Renderer2DData;


class Renderer2D
{
    static void UploadBatch();

public:

    static void Init();

    static void Shutdown();

    static void BeginScene(const Camera2D& camera);

    static void DrawRect(Rect2D& quad);

    // static void DrawText(std::string_view string, float scale, v3 color, v2 position, float zIndex = 0.0f);

    static void EndScene();

};

}