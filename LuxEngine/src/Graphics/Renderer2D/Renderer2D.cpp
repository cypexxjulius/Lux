#include "Renderer2D.h"
#include <Graphics/Renderer.h>


#pragma warning(disable: 4201)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(default: 4201)


#include <Assets/Manager.h>

#include "Renderer2DBatches.h"

namespace Lux
{

struct Renderer2DData
{
    RectBatch rect_batch;
    GlyphBatch glyph_batch;
};

static Renderer2DData* Data = nullptr;

void Renderer2D::Init()
{
    Verify(Data == nullptr);

    Data = new Renderer2DData();

    Renderer::get_api().enable_debug();

    Renderer::get_api().enable(RenderingOption::BLEND);
    Renderer::get_api().set_blend_function(BlendFunc::SRC_ALPHA, BlendFunc::ONE_MINUS_SRC_ALPHA);

    Renderer::get_api().enable(RenderingOption::DEPTH_TEST);
    Renderer::get_api().enable(RenderingOption::MULTISAMPLE);

}

void Renderer2D::Shutdown()
{
    Verify(Data);

    delete Data;
    Data = nullptr;
}

void Renderer2D::BeginScene(const mat4& projection)
{
    Verify(Data)

    Data->rect_batch.begin(projection);
    Data->glyph_batch.begin(projection);

}

void Renderer2D::EndScene()
{
    Verify(Data);

    Data->glyph_batch.end();
    Data->rect_batch.end();

}

void Renderer2D::DrawTexturedRect(const mat4& transform, const v4& color, const Ref<Texture>& texture, float tiling)
{
    Verify(Data);

    u32 TextureID = Data->rect_batch.register_texture(texture->bitmap());

    Data->rect_batch.Push(transform, color, TextureID, tiling);
}

void Renderer2D::DrawBitmap(const mat4& transform, const v4& color, const Ref<Bitmap>& texture, const std::array<v2, 4>& tex_coords)
{
    Verify(Data);

    u32 TextureID = Data->rect_batch.register_texture(texture);

    Data->rect_batch.Push(transform, color, TextureID, tex_coords);
}

void Renderer2D::DrawRect(const mat4& transform, const v4& color)
{
    Verify(Data);

    Data->rect_batch.Push(transform, color, 0, 1.0f);
}

void Renderer2D::DrawText(std::string_view text, const mat4& transform, const v4& color, Ref<Font>& font)
{
    Verify(Data);

    u32 TextureID = Data->glyph_batch.register_texture(font->bitmap());

    double linelength = 0.0;

    for (auto character : text)
    {
        auto& glyph = font->getGlyph(character);
            

        if (character != ' ')
            Data->glyph_batch.Push(transform * glm::translate(glm::mat4(1.0f), { linelength, 0.0f, 0.0f }) * glyph.transform, color, glyph.text_coords, TextureID);
        
        linelength += glyph.advance;

    }
}

}