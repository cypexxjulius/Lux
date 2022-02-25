#include "Renderer2D.h"
#include "Renderer/RendererAPI.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils/Debug.h"

namespace Lux
{

u32          Renderer2D::s_QuadIndexCount = 0;
QuadVertex   Renderer2D::s_QuadVertexBufferBase[MAX_VERTICES] = { };
QuadVertex*  Renderer2D::s_QuadVertexStackPtr = nullptr;

u32 const    Renderer2D::s_TextureSampler[MAX_TEXTURES] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
    21,22,23,24,25,26,27,28,29
};

float        Renderer2D::s_SceneZIndex = 0;

VertexArray* Renderer2D::s_QuadVertexArray; 
Texture2D*    Renderer2D::s_IdentityTexture;

Texture2D*  Renderer2D::s_TextureSlots[MAX_TEXTURES] {};
u32         Renderer2D::s_TextureSlotIndex = 0;
const Font*       Renderer2D::s_UsedFont = nullptr;


void Renderer2D::init()
{
    s_QuadVertexArray = VertexArray::create();

    VertexBuffer* vb = VertexBuffer::create(MAX_VERTICES * sizeof(QuadVertex));

    
    vb->set_layout({
        { "a_Position", DataType::FLOAT, 3 },
        { "a_Color", DataType::FLOAT, 4},
        { "a_TexCoord", DataType::FLOAT, 2},
        { "a_TextureSlot", DataType::FLOAT, 1},
        { "a_TilingFactor", DataType::FLOAT, 1}
    });
    

    s_QuadVertexArray->add_vertex_buffer(vb);

    u32 QuadIndices[MAX_INDICES];

    u32 offset = 0;
    for(u32 i = 0; i < MAX_INDICES; i +=6)
    {
        QuadIndices[i + 0] = offset + 0;
        QuadIndices[i + 1] = offset + 1;
        QuadIndices[i + 2] = offset + 2;

        QuadIndices[i + 3] = offset + 2;
        QuadIndices[i + 4] = offset + 3;
        QuadIndices[i + 5] = offset + 0;

        offset += 4;       
    }

    s_QuadVertexArray->set_index_buffer(IndexBuffer::create(QuadIndices, MAX_INDICES));

    u32 WhiteTextureData = 0xffffffff;
    s_IdentityTexture = new Texture2D( ImageType::RGBA, 1, 1, (void*)&WhiteTextureData );
}

void Renderer2D::shutdown()
{
    delete s_QuadVertexArray;
    s_QuadVertexArray = nullptr;

    delete s_IdentityTexture;
    s_IdentityTexture = nullptr;
}

void Renderer2D::begin_scene(const Camera2D& camera, const Shader* shader, const Font* font)
{
    s_UsedFont = font;

    shader->bind();

    shader->set_int_array("u_Textures", (int*)&s_TextureSampler, MAX_TEXTURES);

    shader->set_mat4("u_ViewProj", camera.view_proj_mat());

    s_QuadIndexCount = 0;
    s_QuadVertexStackPtr = s_QuadVertexBufferBase;

    s_TextureSlotIndex = 0;

}
void Renderer2D::end_scene()
{
    if(s_QuadIndexCount != 0)
        upload_batch();

    s_UsedFont = nullptr;
}

inline void Renderer2D::upload_batch()
{
    s_IdentityTexture->bind(0);

    if(s_UsedFont)
        s_UsedFont->bind(1);

    for(u8 i = 0; i < s_TextureSlotIndex; i++)
        s_TextureSlots[i]->bind(i + RESERVED_SLOTS);

    u32 vertexBufferSize = (u32)((u8*)s_QuadVertexStackPtr - (u8*)s_QuadVertexBufferBase);
    s_QuadVertexArray->vertexbuffer(0)->set_data(
        s_QuadVertexBufferBase,
        vertexBufferSize
    );  

    RendererAPI::Get()->draw_indexed(s_QuadVertexArray, (u32)s_QuadIndexCount);

    s_QuadIndexCount = 0;
    s_QuadVertexStackPtr = s_QuadVertexBufferBase;

    s_TextureSlotIndex = 0;
}

constexpr void Renderer2D::push_vertices(const v3 position[4], const v4 color, const v2 textureCoords[4], const float textureID, const float tiling)
{
    for(u8 i = 0; i < 4; i++)
    {
        s_QuadVertexStackPtr->Position = position[i];                               
        s_QuadVertexStackPtr->Color = color;                                         
        s_QuadVertexStackPtr->TexCoord = textureCoords[i];                           
        s_QuadVertexStackPtr->TextureSlot = textureID;        
        s_QuadVertexStackPtr->Tiling = tiling;                         
        s_QuadVertexStackPtr++;                                                     
    }
    s_QuadIndexCount += 6; 
}


void Renderer2D::draw_quad(Quad2D&& quad)
{
    if((s_QuadIndexCount + 6 >= MAX_INDICES) || (quad.texture != nullptr && s_TextureSlotIndex == 31))
        upload_batch();

    float TextureID = 0;
    if(quad.texture != nullptr)
    {
        for(u8 i = 0; i < s_TextureSlotIndex; i++)
        {
            if(s_TextureSlots[i]->id() == quad.texture->id())
                TextureID = (float)i + RESERVED_SLOTS;
                break;
        }

        TextureID = (float)s_TextureSlotIndex + RESERVED_SLOTS;
        s_TextureSlots[s_TextureSlotIndex++] = quad.texture;
    }

    const float const_TextureID = TextureID;
    const v4 color = quad.color;
    const float tiling = quad.tiling ? quad.tiling : 1.0f; 

    s_SceneZIndex += ZINDEX_STEPS;
    quad.zIndex += s_SceneZIndex;

    const mat4 transform = glm::translate(glm::mat4(1.0f), { quad.position.x, quad.position.y, 1.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(quad.rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { quad.width, quad.height, 1.0f });
    
    const v3 positions[4] = {
            transform * QUAD_VERTEX_POSITIONS[0],
            transform * QUAD_VERTEX_POSITIONS[1],
            transform * QUAD_VERTEX_POSITIONS[2],
            transform * QUAD_VERTEX_POSITIONS[3],
    };

    push_vertices(positions, color, TEXTURE_COORDS, const_TextureID, tiling);
}

void Renderer2D::draw_text(std::string_view string, float scale, v3 color, v2 position, float zIndex)
{
    Assert(s_UsedFont != nullptr, "Font must be set");

    if(string.empty())
        return;

    float lineLength = 0.0f;
    
    position.y += s_UsedFont->ascent() * scale;

    s_SceneZIndex += ZINDEX_STEPS;
    zIndex += s_SceneZIndex;

    for(u32 i = 0; i < string.length(); i++)
    {
        auto& charData = (*s_UsedFont)[string[i]];
        

        v2 textureCoords[4] = {
            { charData.x0, charData.y0 },
            { charData.x1, charData.y0 },
            { charData.x1, charData.y1 },
            { charData.x0, charData.y1 }        
        };

        float width = charData.width * scale;
        float height = charData.height * scale;

        lineLength += charData.lsb * scale;

        float posx = position.x + lineLength;
        float posy = position.y + charData.baseline * scale;
        
        const v3 position[4] = {
                { posx,            posy,          zIndex },
                { posx + width,    posy,          zIndex },
                { posx + width,    posy + height, zIndex },
                { posx,            posy + height, zIndex },
        };

        if(string[i] != ' ')
        {
            push_vertices(
                position, 
                { color.x, color.y, color.z, 1.0f }, 
                textureCoords, 
                1, 
                1.0f
            );
        }

        const float kerning = (i != string.length() - 1) ? charData.kerning[string[i+1] - Font::FIRST_CHAR] : 0.0f;
        lineLength += (charData.ax + kerning) * scale;
    }
}


}