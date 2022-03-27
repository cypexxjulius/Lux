#include "Renderer2D.h"
#include "Graphics/Renderer.h"


#pragma warning(disable: 4201)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(default: 4201)


#include "Assets/Manager.h"


namespace Lux
{


struct RectVertex
{
    v3 Position;
    v4 Color;
    v2 TexCoord;
    float TextureSlot;
    float Tiling;
};

constexpr u32 MaxRects = 20000;
constexpr u32 MaxRectVertices = MaxRects * 4;
constexpr u32 MaxRectIndices = MaxRects * 6;
constexpr u32 MaxTextureSlots = 32;

constexpr v4 RectVertexPositions[4] = {
    {  0.0f,  0.0f, 0.0f, 1.0f },
	{  1.0f,  0.0f, 0.0f, 1.0f },
	{  1.0f,  -1.0f, 0.0f, 1.0f },
	{  0.0f,  -1.0f, 0.0f, 1.0f },
};

constexpr const v2 RectTextureCoords[4] = {
    {0, 0},
    {1, 0},
    {1, 1},
    {0, 1}
};

struct Renderer2DData
{
    
    Ref<VertexArray>    rect_vertex_array;
    Ref<VertexBuffer>   rect_vertex_buffer;

    u32 rect_index_count = 0;

    std::vector<RectVertex> rect_vertexes;
    std::vector<Ref<Texture>> texture_slots;

};

static Renderer2DData* RendererData = nullptr;

void Renderer2D::Init()
{
    Verify(RendererData == nullptr);
    
    RendererData = new Renderer2DData;
    RendererData->rect_vertexes.reserve(MaxRectVertices);
    RendererData->texture_slots.reserve(MaxTextureSlots - 1);
    
    RendererData->rect_vertex_array = VertexArray::Create();

    VertexBuffer* vb = VertexBuffer::Create(MaxRects * sizeof(MaxRectVertices));

    
    vb->set_layout({
        { "a_Position", DataType::FLOAT, 3 },
        { "a_Color", DataType::FLOAT, 4},
        { "a_TexCoord", DataType::FLOAT, 2},
        { "a_TextureSlot", DataType::FLOAT, 1},
        { "a_TilingFactor", DataType::FLOAT, 1}
    });
    

    RendererData->rect_vertex_array->add_vertex_buffer(vb);

    u32* QuadIndices = new u32[MaxRectIndices];

    u32 offset = 0;
    for(u32 i = 0; i < MaxRectIndices; i +=6)
    {
        QuadIndices[i + 0] = offset + 0;
        QuadIndices[i + 1] = offset + 1;
        QuadIndices[i + 2] = offset + 2;

        QuadIndices[i + 3] = offset + 2;
        QuadIndices[i + 4] = offset + 3;
        QuadIndices[i + 5] = offset + 0;

        offset += 4;       
    }

    RendererData->rect_vertex_array->set_index_buffer(IndexBuffer::Create(QuadIndices, MaxRectIndices));

    delete[] QuadIndices;

    u32 WhiteTextureData = UINT32_MAX;
    
    ResourceManager::CreateTexture("IdentityTexture", { ImageType::RGBA, 1, 1, (void*)&WhiteTextureData });

    std::vector<int> samplers;
    samplers.reserve(MaxTextureSlots);

    for(u32 i = 0; i < MaxTextureSlots; i++)
        samplers[i] = i;

    auto shader = ResourceManager::CreateShader("TextureShader", { "res/shaders/TextureShader.glsl" });
    shader->upload_int_array("u_Textures", samplers);
}

void Renderer2D::Shutdown()
{
    Verify(RendererData != nullptr);
    delete RendererData;
}

void Renderer2D::BeginScene(const Camera2D& camera)
{
    auto shader = ResourceManager::GetShader("TextureShader");
    shader->bind();
    shader->upload_mat4("u_ViewProj", camera.view_proj_mat());


    RendererData->rect_index_count = 0;
    RendererData->rect_vertexes.clear();
    RendererData->texture_slots.clear();
}
void Renderer2D::EndScene()
{
    if(RendererData->rect_index_count != 0)
        UploadBatch();
}

inline void Renderer2D::UploadBatch()
{
    ResourceManager::GetShader("TextureShader")->bind();

    u8 bound_texture_slot = 0;
    ResourceManager::GetTexture("IdentityTexture")->bind(bound_texture_slot++);
    for(auto& texture : RendererData->texture_slots)
        texture->bind(bound_texture_slot++);

    RendererData->rect_vertex_array->vertexbuffer(0)->set_data(
        RendererData->rect_vertexes.data(),
        static_cast<u32>(RendererData->rect_vertexes.size() *  sizeof(RectVertex))
    );  

    Renderer::DrawIndexed(RendererData->rect_vertex_array, (u32)RendererData->rect_index_count);

    RendererData->rect_index_count = 0;
    RendererData->rect_vertexes.clear();
    RendererData->texture_slots.clear();
}

inline void PushVertices(v3 position[4], v4 color, const v2 textureCoords[4], float textureID, float tiling)
{
    for(u8 i = 0; i < 4; i++)
    {
        RendererData->rect_vertexes.emplace_back(
            position[i],
            color,
            textureCoords[i],
            textureID,
            tiling
        );                                                  
    }

    RendererData->rect_index_count += 6; 
}


void Renderer2D::DrawRect(Rect2D& quad)
{
    if((RendererData->rect_index_count + 6 >= MaxRectIndices) || (quad.texture != nullptr && RendererData->texture_slots.capacity() == RendererData->texture_slots.size()))
        UploadBatch();

    float TextureID = 0;
    if(quad.texture != nullptr)
    {
        float count = 0;
        for(auto& texture : RendererData->texture_slots)
        {
            if(texture->id() == quad.texture->id())
            {
                TextureID = count;
                break;
            }
            count++;
        }

        RendererData->texture_slots.push_back(quad.texture);
        TextureID = count;
    }

    const v4 color = quad.color;
    const float tiling = quad.tiling ? quad.tiling : 1.0f; 


    const mat4 transform = glm::translate(glm::mat4(1.0f), { quad.position.x, quad.position.y, 1.0f })
			                * glm::rotate(glm::mat4(1.0f), glm::radians(quad.rotation), { 0.0f, 0.0f, 1.0f })
			                * glm::scale(glm::mat4(1.0f), { quad.width, quad.height, 1.0f });
    
    v3 positions[4] = {
            transform * RectVertexPositions[0],
            transform * RectVertexPositions[1],
            transform * RectVertexPositions[2],
            transform * RectVertexPositions[3],
    };

    PushVertices(positions, color, RectTextureCoords, TextureID, tiling);
}


}