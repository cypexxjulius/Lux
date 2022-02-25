#pragma once 

#include "Utils/Types.h"

#include "Shader.h"
#include "Camera2D.h"
#include "Assets/Font.h"
#include "Assets/Texture2D.h"
#include "VertexArray.h"

namespace Lux
{

struct Quad2D
{
    float width;
    float height;
    float tiling;
    float zIndex;
    float rotation;
    Texture2D* texture;
    v2 position;
    v4 color;
};


struct QuadVertex
{
    v3 Position;
    v4 Color;
    v2 TexCoord;
    float TextureSlot;
    float Tiling;
};


class Renderer2D
{

private:
    static constexpr u32 MAX_QUADS = 30000;
    static constexpr u32 MAX_VERTICES = MAX_QUADS * 4;
    static constexpr u32 MAX_INDICES = MAX_QUADS * 6;

    static constexpr u8 RESERVED_SLOTS = 2;
    static constexpr u32 MAX_TEXTURES = 32 - RESERVED_SLOTS; 

    static constexpr f32 ZINDEX_STEPS = 1.0f / (float)MAX_QUADS;

    static constexpr v2 TEXTURE_COORDS[4] = {
        { 0, 0 },
        { 1, 0 },
        { 1, 1 },
        { 0, 1 }
    };

    static constexpr v4 QUAD_VERTEX_POSITIONS[4] = {
        {  0.0f,  0.0f,  0.0f, 1.0f },
        {  1.0f,  0.0f,  0.0f, 1.0f },
        {  1.0f,  -1.0f, 0.0f, 1.0f },
        {  0.0f,  -1.0f, 0.0f, 1.0f },
    }; 


    static u32 s_QuadIndexCount;
    static QuadVertex s_QuadVertexBufferBase[MAX_VERTICES];
    static QuadVertex* s_QuadVertexStackPtr;
    static const Font* s_UsedFont;

    static const u32 s_TextureSampler[MAX_TEXTURES];
    static Texture2D* s_TextureSlots[MAX_TEXTURES];
    static u32 s_TextureSlotIndex;

    static float s_SceneZIndex;

    static VertexArray* s_QuadVertexArray; 
    static Texture2D* s_IdentityTexture;

private:

    static void upload_batch();

    static constexpr void push_vertices(const v3 position[4], const v4 color, const v2 textureCoords[4], const float textureID, const float tiling);

public:

    static void init();

    static void shutdown();

    static void begin_scene(const Camera2D& camera, const Shader* shader, const Font* font = nullptr);

    static void draw_quad(Quad2D&& quad);

    static void draw_text(std::string_view string, float scale, v3 color, v2 position, float zIndex = 0.0f);

    static void end_scene();

};

}