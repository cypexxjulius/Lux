#include "Renderer2D.h"
#include "Graphics/Renderer.h"


#pragma warning(disable: 4201)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(default: 4201)




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
    Ref<Shader>         texture_shader;
    Ref<Texture2D>      white_texture;

    u32         rect_index_count = 0;
    RectVertex* rect_vertex_buffer_base;
    RectVertex* rect_vertex_buffer_ptr;

    Array<Ref<Texture2D>, MaxTextureSlots> texture_slots;
    // 0 is reserved 
    u32 texture_slots_index = 1; 

};

static Renderer2DData* RendererData;

void Renderer2D::Init()
{
    RendererData = new Renderer2DData;
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
    RendererData->white_texture = std::make_shared<Texture2D>(ImageType::RGBA, 1, 1, (void*)&WhiteTextureData);

    u32* samplers = new u32[MaxTextureSlots];

    for(u32 i = 0; i < MaxTextureSlots; i++)
        samplers[i] = i;

    delete[] samplers;

    RendererData->texture_shader = Shader::Create("TextureShader", "res/shaders/TextureShader.glsl");
    

}

void Renderer2D::Shutdown()
{
    delete RendererData;
}

void Renderer2D::BeginScene(const Camera2D& camera)
{

    RendererData->texture_shader->bind();
    RendererData->texture_shader->set_mat4("u_ViewProj", camera.view_proj_mat());
    RendererData->texture_shader->set_int_array("u_Textures", (int*)&RendererData->texture_slots, MaxTextureSlots);


    RendererData->rect_index_count = 0;
    RendererData->rect_vertex_buffer_ptr = RendererData->rect_vertex_buffer_base;

    RendererData->texture_slots_index = 1;
}
void Renderer2D::EndScene()
{
    if(RendererData->rect_index_count != 0)
        UploadBatch();
}

inline void Renderer2D::UploadBatch()
{
    RendererData->white_texture->bind(0);

    for(u8 i = 0; i < RendererData->texture_slots_index; i++)
        RendererData->texture_slots[i]->bind(i);

    u32 vertexBufferSize = (u32)((u8*)RendererData->rect_vertex_buffer_ptr - (u8*)RendererData->rect_vertex_buffer_base);
    RendererData->rect_vertex_array->vertexbuffer(0)->set_data(
        RendererData->rect_vertex_buffer_base,
        vertexBufferSize
    );  

    Renderer::DrawIndexed(RendererData->rect_vertex_array, (u32)RendererData->rect_index_count);

    RendererData->rect_index_count = 0;
    RendererData->rect_vertex_buffer_ptr = RendererData->rect_vertex_buffer_base;

    RendererData->texture_slots_index = 1;
}

inline void PushVertices(const v3 position[4], const v4 color, const v2 textureCoords[4], const float textureID, const float tiling)
{
    for(u8 i = 0; i < 4; i++)
    {
        RendererData->rect_vertex_buffer_ptr->Position = position[i];                               
        RendererData->rect_vertex_buffer_ptr->Color = color;                                         
        RendererData->rect_vertex_buffer_ptr->TexCoord = textureCoords[i];                           
        RendererData->rect_vertex_buffer_ptr->TextureSlot = textureID;        
        RendererData->rect_vertex_buffer_ptr->Tiling = tiling;                         
        RendererData->rect_vertex_buffer_ptr++;                                                     
    }
    RendererData->rect_index_count += 6; 
}


void Renderer2D::DrawRect(Rect2D& quad)
{
    if((RendererData->rect_index_count + 6 >= MaxRectIndices) || (quad.texture != nullptr && RendererData->texture_slots_index == 31))
        UploadBatch();

    float TextureID = 0;
    if(quad.texture != nullptr)
    {
        for(u8 i = 1; i < RendererData->texture_slots_index; i++)
        {
            if(RendererData->texture_slots[i]->id() == quad.texture->id())
                TextureID = (float)i;
                break;
        }

        TextureID = (float)RendererData->texture_slots_index;
        RendererData->texture_slots[RendererData->texture_slots_index++] = quad.texture;
    }

    const float const_TextureID = TextureID;
    const v4 color = quad.color;
    const float tiling = quad.tiling ? quad.tiling : 1.0f; 


    const mat4 transform = glm::translate(glm::mat4(1.0f), { quad.position.x, quad.position.y, 1.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(quad.rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { quad.width, quad.height, 1.0f });
    
    const v3 positions[4] = {
            transform * RectVertexPositions[0],
            transform * RectVertexPositions[1],
            transform * RectVertexPositions[2],
            transform * RectVertexPositions[3],
    };

    PushVertices(positions, color, RectTextureCoords, const_TextureID, tiling);
}


}