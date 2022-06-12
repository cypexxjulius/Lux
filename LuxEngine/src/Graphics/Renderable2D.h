#pragma once 

#include "Renderer2D.h"

#include "Utils/Types.h"
#include "Utils/Assert.h"

#include <array>
#include <vector>
#include <initializer_list>



#include <Assets/Manager.h>

namespace Lux
{

enum class Renderable2DType
{
    None,
    Rect,
    Circle,
    Text
};

static const std::array<v2, 4> RectTextureCoords = {
    v2 { 0.0f, 0.0f },
    v2 { 1.0f, 0.0f },
    v2 { 1.0f, 1.0f },
    v2 { 0.0f, 1.0f }
};

template<Renderable2DType TYPE>
class Renderable2D
{
private:

    Renderable2D() = default;
};

template<>
class Renderable2D<Renderable2DType::Rect>
{
private:

    static constexpr v4 VertexPositions[4] {
        { 0, 1, 0, 1 },
        { 1, 1, 0, 1 },
        { 1, 0, 0, 1 },
        { 0, 0, 0, 1 },
    };

    mat4 m_transform;
    std::array<v2, 4> m_texture_coords;

    Ref<Texture> m_texture;
    v4 m_color;
    float m_tiling;

public:

    friend class Renderer2D;

    Renderable2D(v2 position, float width, float height, const v4& color, float tiling = 1.0f)
        :   m_transform(glm::translate(glm::mat4(1.0f), { position.x, position.y, 1.0f }) * glm::scale(glm::mat4(1.0f), { width, height, 1.0f })),
            m_texture_coords(RectTextureCoords),
            m_color(color),
            m_tiling(tiling)
    {
    }

    inline void set_texture(const Ref<Texture>& texture)
    {
        m_texture = texture;
    }

    inline void set_dimensions(v2 position, float width, float height) 
    {
        m_transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 1.0f }) * glm::scale(glm::mat4(1.0f), { width, height, 1.0f });
    }

    inline void set_color(const v4& color)
    {
        m_color = color;
    }

    inline void set_texture(const Ref<Texture>& texture, float tiling = 1.0f)
    {
        m_texture = texture;
        m_tiling = tiling;
    }

    inline void reset_transform(v2 position, float width, float height)
    {
        m_transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 1.0f }) * glm::scale(glm::mat4(1.0f), { width, height, 1.0f });
    }
};


template<>
class Renderable2D<Renderable2DType::Text>
{
private:

    static constexpr v4 VertexPositions[4] {
        { 0, 1, 0, 1 },
        { 1, 1, 0, 1 },
        { 1, 0, 0, 1 },
        { 0, 0, 0, 1 },
    };

    v4 m_color;
    u32 m_count = 0;

    float m_width, m_height;
    
    std::vector<std::pair<mat4, std::array<v2, 4>>> m_glyphs;

public:

    friend class Renderer2D;

    Renderable2D(std::string_view string, v2 position, float scale, const v3 color = { 1.0f, 1.0f, 1.0f })
        : m_color(color, 1.0f), m_count(static_cast<u32>(string.length()))
    {
        if(string.empty())
            return;

        auto font = ResourceManager::GetFont("StandardFont");


        scale *= 0.01f;
        float lineLength = 0.0f;
        position.y += font->ascent() * scale;

        m_height = font->lineheight() * scale;

        m_glyphs.reserve(m_count);

        for(u32 i = 0; i < string.length(); i++)
        {

            auto& glyph = font->operator[](string[i]);


            lineLength += glyph.lsb * scale;

            if(string[i] != ' ')
            {
                m_glyphs.push_back({
                    {
                        glm::translate(glm::mat4(1.0f), { position.x + lineLength, position.y + glyph.baseline * scale, 1.0f })
                        * glm::scale(glm::mat4(1.0f), { glyph.width * scale, glyph.height * scale, 1.0f })
                    },
                    {
                        v2 { glyph.x0, glyph.y1 },
                        v2 { glyph.x1, glyph.y1 },
                        v2 { glyph.x1, glyph.y0 },
                        v2 { glyph.x0, glyph.y0 }        
                    }
                });

            }
            else 
            {
                m_count--;
            }

            lineLength += glyph.ax * scale;

            if(i + 1 == string.length())
            {
                m_width = lineLength;
                return;
            }
            lineLength += glyph.kerning[string[i + 1] - Font::FIRST_CHAR] * scale;
        }
    }

    inline void set_color(const v3 color)
    {
        m_color = { color, 1.0f };
    }

    inline void reset_transform(std::string_view string, v2 position, float scale)
    {
        if(string.empty())
        {
            m_count = 0;
            return;
        }

        auto font = ResourceManager::GetFont("StandardFont");
        scale *= 0.01f;
        float lineLength = 0.0f;
        position.y += font->ascent() * scale;
        
        m_glyphs.clear();
        m_glyphs.reserve(m_count);

        m_height = font->lineheight() * scale;
        
        for(u32 i = 0; i < string.length(); i++)
        {

            auto& glyph = font->operator[](string[i]);

            if(i != 0)
                lineLength += glyph.kerning[string[i - 1] - Font::FIRST_CHAR] * scale;

            lineLength += glyph.lsb * scale;

            if(string[i] != ' ')
            {
                m_glyphs.push_back({
                    {
                        glm::translate(glm::mat4(1.0f), { position.x + lineLength, position.y + glyph.baseline * scale, 1.0f })
                        * glm::scale(glm::mat4(1.0f), { glyph.width * scale, glyph.height * scale, 1.0f })
                    },
                    {
                        v2 { glyph.x0, glyph.y1 },
                        v2 { glyph.x1, glyph.y1 },
                        v2 { glyph.x1, glyph.y0 },
                        v2 { glyph.x0, glyph.y0 }        
                    }
                });

            }
            else 
            {
                m_count--;
            }

            lineLength += glyph.ax * scale;
        }
        m_width = lineLength;

    }

};


}