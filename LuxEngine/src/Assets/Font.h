#pragma once 

#include <string_view>
#include <memory>

#include "Renderer/Bitmap.h"
#include "Utils/Assert.h"

#define MAX_CHAR_COUNT '~' - ' ' + 1

namespace Lux
{

struct CharData
{
    float x0, x1, y0, y1;
    float width, height;
    float baseline;
    float ax, lsb;

    f32 kerning[MAX_CHAR_COUNT];
};



class Font
{
private:
    static constexpr u32 CHAR_COUNT = MAX_CHAR_COUNT;
    static constexpr u32 BITMAP_WIDTH = 1024;
    static constexpr u32 BITMAP_HEIGHT = 4024;
    Bitmap* m_Bitmap = nullptr;

    float m_Ascent, m_Descent; 
    float m_LineHeight, m_FontSize, m_LineGap;
    CharData m_CharData[CHAR_COUNT];

    void make_self(const std::string& file_path);

public:
    static constexpr u8 LAST_CHAR = '~' + 1;
    static constexpr u8 FIRST_CHAR = ' ';

    static constexpr u32 char_to_glyph(u32 character)
    { return character -= FIRST_CHAR; }

    Font(const std::string& file_path)
    {
        make_self(file_path);
    }

    inline void create_new(const std::string& file_path)
    {
        if(m_Bitmap != nullptr)
            delete m_Bitmap;
            
        make_self(file_path);
    };

    Font() {}

    Font(Font&& other_font)
    : m_Bitmap(std::exchange(other_font.m_Bitmap, nullptr))
    , m_Ascent(other_font.m_Ascent)
    , m_Descent(other_font.m_Descent)
    , m_LineHeight(other_font.m_LineHeight)
    , m_FontSize(other_font.m_FontSize)
    , m_LineGap(other_font.m_LineGap)
    {
        std::memcpy(m_CharData, other_font.m_CharData, sizeof(CharData[CHAR_COUNT]));
    };

    ~Font()
    {
        if(m_Bitmap != nullptr)
            delete m_Bitmap;
    }

    inline void bind(u8 slot) const
    { m_Bitmap->bind(slot); };

    inline const CharData& operator[](int index) const
    {
        if(index >= 0 && index < UINT8_MAX)
            return m_CharData[char_to_glyph(index)];

        TODO();
        return m_CharData[0];
    }

    inline float lineheight() const
    { return m_LineHeight; }

    inline float linegap() const 
    { return m_LineGap; }

    inline float ascent() const 
    { return m_Ascent; }

    inline float descent() const 
    { return m_Descent; }


    inline float fontsize() const 
    { return m_FontSize; }

};

}