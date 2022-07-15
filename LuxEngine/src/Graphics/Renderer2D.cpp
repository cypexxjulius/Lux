#include "Renderer2D.h"
#include <Graphics/Renderer.h>


#pragma warning(disable: 4201)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(default: 4201)


#include <Assets/Manager.h>

#include <Utils/Types.h>

struct RectBatch
{
    Ref<VertexArray> va;
};


/*
namespace Lux
{

constexpr u32 MaxElements = 20000;
constexpr u32 MaxVertices = MaxElements * 4;
constexpr u32 MaxIndices = MaxElements * 6;
constexpr u32 MaxTextureSlots = 30;

struct Renderer2DData
{
    
    Ref<VertexArray>    rect_vertex_array;
    Ref<VertexBuffer>   rect_vertex_buffer;

    u32 rect_index_count = 0;

    std::vector<Renderer2DVertex> rect_vertexes;
    std::vector<Ref<Texture>> texture_slots;

    Ref<Shader> active_shader;
    Ref<Font> active_font;

    mat4* transform_stack_back;
    std::vector<mat4> transform_stack;
};

static Renderer2DData* RendererData = nullptr;

void Renderer2D::Init()
{
    Verify(RendererData == nullptr);
    
    RendererData = new Renderer2DData;
    RendererData->rect_vertexes.reserve(MaxVertices);
    RendererData->texture_slots.reserve(MaxTextureSlots);

    RendererData->transform_stack.emplace_back(1.0f);
    RendererData->transform_stack_back = &RendererData->transform_stack.back(); 

    RendererData->rect_vertex_array = VertexArray::Create();

    VertexBuffer* vb = VertexBuffer::Create(MaxVertices * sizeof(Renderer2DVertex));

    
    vb->set_layout({
        { "a_Position", DataType::FLOAT, 3 },
        { "a_Color", DataType::FLOAT, 4},
        { "a_TexCoord", DataType::FLOAT, 2},
        { "a_TextureSlot", DataType::FLOAT, 1},
        { "a_TilingFactor", DataType::FLOAT, 1}
    });
    

    RendererData->rect_vertex_array->add_vertex_buffer(vb);

    u32* QuadIndices = new u32[MaxIndices];

    u32 offset = 0;
    for(u32 i = 0; i < MaxIndices; i +=6)
    {
        QuadIndices[i + 0] = offset + 0;
        QuadIndices[i + 1] = offset + 1;
        QuadIndices[i + 2] = offset + 2;

        QuadIndices[i + 3] = offset + 2;
        QuadIndices[i + 4] = offset + 3;
        QuadIndices[i + 5] = offset + 0;

        offset += 4;       
    }

    RendererData->rect_vertex_array->set_index_buffer(IndexBuffer::Create(QuadIndices, MaxIndices));

    delete[] QuadIndices;

 
    ResourceManager::CreateFont("StandardFont", { "EngineLayer/res/fonts/Roboto-Black.ttf" });
   
    std::vector<int> samplers;
    samplers.reserve(MaxTextureSlots + 2);

    for(u32 i = 0; i < MaxTextureSlots + 2; i++)
        samplers.push_back(i);

    RendererData->active_shader = ResourceManager::CreateShader("TextureShader", { "EngineLayer/res/shaders/TextureShader.glsl" });
    RendererData->active_shader->bind();
    RendererData->active_shader->upload_int_array("u_Textures", samplers);
}

void Renderer2D::Shutdown()
{
    Verify(RendererData != nullptr);
    
    ResourceManager::DeleteFont("StandardFont");
    ResourceManager::DeleteShader("TextureShader");

    delete RendererData;
}

void Renderer2D::BeginScene(const Ref<Camera2D>& camera)
{
    Verify(RendererData != nullptr);

    RendererData->active_shader->bind();
    RendererData->active_shader->upload_mat4("u_ViewProj", camera->view_proj_mat());


    RendererData->rect_index_count = 0;
    RendererData->rect_vertexes.clear();
    RendererData->texture_slots.clear();
}
void Renderer2D::EndScene()
{
    Verify(RendererData != nullptr);

    if(RendererData->rect_index_count != 0)
        UploadBatch();
}

inline void Renderer2D::UploadBatch()
{
    RendererData->active_shader->bind();

    u8 bound_texture_slot = 1;
    ResourceManager::GetFont("StandardFont")->bind(bound_texture_slot++);
    for(auto& texture : RendererData->texture_slots)
        texture->bind(bound_texture_slot++);

    // TODO Create a seperate data structure for this
    std::reverse(RendererData->rect_vertexes.begin(), RendererData->rect_vertexes.end()); 
    RendererData->rect_vertex_array->vertexbuffer(0)->set_data(
        RendererData->rect_vertexes.data(),
        static_cast<u32>(RendererData->rect_vertexes.size() *  sizeof(Renderer2DVertex))
    );  

    Renderer::DrawIndexed(RendererData->rect_vertex_array, (u32)RendererData->rect_index_count);

    RendererData->rect_index_count = 0;
    RendererData->rect_vertexes.clear();
    RendererData->texture_slots.clear();
}

void Renderer2D::DrawRect(const Renderable2D<Renderable2DType::Rect>& rect)
{
    Verify(RendererData != nullptr);

    auto& textures = RendererData->texture_slots;
    if((RendererData->rect_index_count + 6 >= MaxIndices) || (rect.m_texture != nullptr && textures.capacity() == textures.size()))
        UploadBatch();

    float TextureID = 0;
    if(rect.m_texture != nullptr)
    {
        auto it = std::find(textures.begin(), textures.end(), rect.m_texture);

        if(it == textures.end())
        {
            TextureID = static_cast<float>(textures.size());
            textures.push_back(rect.m_texture);
        }
        else 
            TextureID = static_cast<float>(std::distance(textures.begin(), it));
    }


    const mat4& transform = *RendererData->transform_stack_back * rect.m_transform;
    const auto& texture_coords = rect.m_texture_coords;

    RendererData->rect_vertexes.emplace_back(
        transform * Renderable2D<Renderable2DType::Rect>::VertexPositions[0],
        rect.m_color,
        texture_coords[0],
        TextureID,
        rect.m_tiling
    );

    RendererData->rect_vertexes.emplace_back(
        transform * Renderable2D<Renderable2DType::Rect>::VertexPositions[1],
        rect.m_color,
        texture_coords[1],
        TextureID,
        rect.m_tiling
    ); 

    RendererData->rect_vertexes.emplace_back(
        transform * Renderable2D<Renderable2DType::Rect>::VertexPositions[2],
        rect.m_color,
        texture_coords[2],
        TextureID,
        rect.m_tiling
    ); 

    RendererData->rect_vertexes.emplace_back(
        transform * Renderable2D<Renderable2DType::Rect>::VertexPositions[3],
        rect.m_color,
        texture_coords[3],
        TextureID,
        rect.m_tiling
    );                                                  
            
    RendererData->rect_index_count += 6; 
}

void Renderer2D::DrawText(const Renderable2D<Renderable2DType::Text>& text)
{
    Verify(RendererData != nullptr);
    
    if((RendererData->rect_index_count + 6 * text.m_count >= MaxIndices))
        UploadBatch();

    float TextureID = 1.0f;

    for(const auto&[glyph_transform, texture_coords] : text.m_glyphs)
    {
        mat4 transform = *RendererData->transform_stack_back * glyph_transform;

        RendererData->rect_vertexes.emplace_back(
            transform * Renderable2D<Renderable2DType::Text>::VertexPositions[0],
            text.m_color,
            texture_coords[0],
            TextureID,
            1.0f
        );

        RendererData->rect_vertexes.emplace_back(
            transform * Renderable2D<Renderable2DType::Text>::VertexPositions[1],
            text.m_color,
            texture_coords[1],
            TextureID,
            1.0f
        ); 

        RendererData->rect_vertexes.emplace_back(
            transform * Renderable2D<Renderable2DType::Text>::VertexPositions[2],
            text.m_color,
            texture_coords[2],
            TextureID,
            1.0f
        ); 

        RendererData->rect_vertexes.emplace_back(
            transform * Renderable2D<Renderable2DType::Text>::VertexPositions[3],
            text.m_color,
            texture_coords[3],
            TextureID,
            1.0f
        );                                                  
            
        RendererData->rect_index_count += 6; 
    }
}

void Renderer2D::PushTransform(mat4 transform, bool transform_override)
{
    if (transform_override)
        RendererData->transform_stack.push_back(transform);
	else
		RendererData->transform_stack.push_back(RendererData->transform_stack.back() * transform);

	RendererData->transform_stack_back = &RendererData->transform_stack.back();
}

void Renderer2D::PopTransform()
{
    if (RendererData->transform_stack.size() > 1)
			RendererData->transform_stack.pop_back();

	RendererData->transform_stack_back = &RendererData->transform_stack.back();
}

}
*/