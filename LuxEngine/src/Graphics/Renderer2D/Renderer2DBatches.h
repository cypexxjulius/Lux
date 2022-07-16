#pragma once

#include "Utils/Types.h"

#include "Graphics/Core/Bitmap.h"
#include "Graphics/Core/VertexArray.h"
#include "Graphics/Core/Buffer.h"
#include "Graphics/Renderer.h"

#include "Assets/Shader.h"
#include "Assets/Manager.h"

namespace Lux
{


constexpr u32 MaxElements = 20000;
constexpr u32 MaxVertices = MaxElements * 4;
constexpr u32 MaxIndices = MaxElements * 6;
constexpr u32 MaxTextureSlots = 32;


struct RectVertex
{
    v3 Position;
    v4 Color;
    v2 TexCoord;
    float TextureSlot;
    float Tiling;
};

struct GlyphVertex
{
    v3 Position;
    v4 Color;
    v2 TexCoord;
    float FontSlot;
};




template<typename T>
struct Renderer2DBatch
{
protected:

    Ref<VertexArray> va;
    Ref<Shader> shader;
    
    std::vector<T> vertices;
    std::vector<Ref<Bitmap>> textures;
    u32 index_count;

    Renderer2DBatch(std::initializer_list<VertexBufferElement> vb_elements)
    {
        vertices.reserve(MaxVertices);
        vertices.reserve(MaxTextureSlots);

        va = VertexArray::Create();

        auto* vb = VertexBuffer::Create(MaxVertices * sizeof(T));
        vb->set_layout(vb_elements);

        va->add_vertex_buffer(vb);

            
        u32* QuadIndices = new u32[MaxIndices];

        u32 offset = 0;

        for (u32 i = 0; i < MaxIndices; i += 6)
        {
            QuadIndices[i + 0] = offset + 0;
            QuadIndices[i + 1] = offset + 1;
            QuadIndices[i + 2] = offset + 2;

            QuadIndices[i + 3] = offset + 2;
            QuadIndices[i + 4] = offset + 3;
            QuadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        va->set_index_buffer(IndexBuffer::Create(QuadIndices, MaxIndices));

        delete[] QuadIndices;
    }
public:

    inline void upload()
    {
        if (!index_count)
            return;

        prep_upload();

        va->vertexbuffer(0)->set_data(
            vertices.data(),
            static_cast<u32>(vertices.size() * sizeof(T))
        );

        Renderer::DrawIndexed(va, index_count);
        
        index_count = 0;
        vertices.clear();
        textures.clear();
    }

    inline void begin(const mat4& projection)
    {
        index_count = 0;
        vertices.clear();
        textures.clear();

        on_begin(projection);
    }

    inline void end()
    {
        upload();
    }

    inline void push_vertex(T&& vertex)
    {
        vertices.emplace_back(std::forward<T>(vertex));
    }

    inline u32 register_texture(const Ref<Bitmap>& bitmap)
    {
        auto it = std::find(textures.begin(), textures.end(), bitmap);
        if (it == textures.end())
        {
            if (textures.size() == MaxTextureSlots)
            {
                upload();
                textures.push_back(bitmap);
                return textures.size() - 1;
            }

            textures.push_back(bitmap);
            return textures.size() - 1;
        }
        
        return std::distance(textures.begin(), it);
    }


    virtual void prep_upload() = 0;

    virtual void on_begin(const mat4& projection) = 0;
};

struct RectBatch final : public Renderer2DBatch<RectVertex>
{
private:
        
    Ref<Texture> standard_color;

    static constexpr v4 VertexPositions[4]{
        { 0, 1, 0, 1 },
        { 1, 1, 0, 1 },
        { 1, 0, 0, 1 },
        { 0, 0, 0, 1 },
    };

    static constexpr v2 TextureCoords[4]{
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

public:
       

    RectBatch()
        :   Renderer2DBatch({
                { "a_Position", DataType::FLOAT, 3 },
                { "a_Color", DataType::FLOAT, 4},
                { "a_TexCoord", DataType::FLOAT, 2},
                { "a_TextureSlot", DataType::FLOAT, 1},
                { "a_TilingFactor", DataType::FLOAT, 1}
            })
    {

        std::vector<int> samplers;
        samplers.reserve(MaxTextureSlots);

        for (u32 i = 0; i < MaxTextureSlots; i++)
            samplers.emplace_back(i);

        shader = ResourceManager::CreateShader("TextureShader", { "res/shaders/TextureShader.glsl" });
        shader->bind();
        shader->upload_int_array("u_Textures", samplers);

        u32 pure_color = UINT32_MAX;
        standard_color = ResourceManager::CreateTexture("PureWhiteTexture", { ImageType::RGBA, 1, 1, &pure_color});
    }

    virtual void on_begin(const mat4& projection) override
    {
        textures.push_back(standard_color->bitmap());
        

        shader->upload_mat4("u_ViewProj", projection);
    }


    virtual void prep_upload() override
    {
        shader->bind();
        
        u32 i = 0;
        for (auto& bitmap : textures)
            bitmap->bind(i++);
        
    }

    inline void Push(const mat4& transform, const v4& color, u32 texture_id, float tiling)
    {
        if (index_count == MaxIndices)
            upload();

        for (u8 i = 0; i < 4; i++)
        {
            push_vertex({
                transform * VertexPositions[i],
                color,
                TextureCoords[i],
                static_cast<float>(texture_id),
                tiling
            });
        }

        index_count += 6;
    }
};

struct GlyphBatch final : public Renderer2DBatch<GlyphVertex>
{
private:

    static constexpr v4 VertexPositions[4]{
        { 0, 1, 0, 1 },
        { 1, 1, 0, 1 },
        { 1, 0, 0, 1 },
        { 0, 0, 0, 1 },
    };

public:


    GlyphBatch()
        : Renderer2DBatch({
                { "a_Position", DataType::FLOAT, 3 },
                { "a_Color", DataType::FLOAT, 4},
                { "a_TexCoord", DataType::FLOAT, 2},
                { "a_FontID", DataType::FLOAT, 1}
            })
    {

        std::vector<int> samplers;
        samplers.reserve(MaxTextureSlots);

        for (u32 i = 0; i < MaxTextureSlots; i++)
            samplers.emplace_back(i);

        shader = ResourceManager::CreateShader("MSDFTextShader", { "res/shaders/MSDFTextShader.glsl" });
        shader->bind();
        shader->upload_int_array("u_Fonts", samplers);
    }

    virtual void on_begin(const mat4& projection) override
    {
        shader->upload_mat4("u_ViewProj", projection);
    }


    virtual void prep_upload() override
    {
        shader->bind();

        u32 i = 0;
        for (auto& bitmap : textures)
            bitmap->bind(i++);

    }

    inline void Push(const mat4& transform, const v4& color, const std::array<v2, 4>& texture_coords, u32 texture_id)
    {
        if (index_count == MaxIndices)
            upload();

        for (u8 i = 0; i < 4; i++)
        {
            push_vertex({
                transform * VertexPositions[i],
                color,
                texture_coords[i],
                static_cast<float>(texture_id)
            });
        }

        index_count += 6;
    }
};



}