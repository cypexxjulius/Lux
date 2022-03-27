#include "Font.h"

#pragma warning(disable: 4505) //Unreferenced formal parameter
#define STBRP_STATIC
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb_rect_pack.h>

#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#pragma warning(default: 4505)

#include "Core/IO.h"

#include "Utils/Assert.h"
#include "Utils/Logger.h"

namespace Lux
{

Font::Font(const std::string& filepath)
    : m_Bitmap(nullptr)
{
    std::string fileContent = IO::read_file(filepath); 

    u8* fileContentPtr = (u8*)fileContent.c_str();


    stbtt_fontinfo FontData;
    stbtt_fontinfo *font = &FontData;

    Verify(stbtt_InitFont(font, fileContentPtr , 0));



    byte* bitmap = new byte[BITMAP_WIDTH * BITMAP_HEIGHT * sizeof(byte)];


    constexpr float fontHeight = 32.0f;
    f32 scale = m_FontSize = stbtt_ScaleForPixelHeight(font, fontHeight); 

    int unscaled_ascent, unscaled_descent, unscaled_linegap;
    stbtt_GetFontVMetrics(font, &unscaled_ascent, &unscaled_descent, &unscaled_linegap);
    


    stbtt_pack_context packContext;
    stbtt_packedchar charData[CHAR_COUNT];

    Verify(stbtt_PackBegin(&packContext, bitmap, BITMAP_WIDTH, BITMAP_HEIGHT, BITMAP_WIDTH, 0, nullptr))

    stbtt_PackSetOversampling(&packContext, 4, 4);

    Verify(stbtt_PackFontRange(&packContext, fileContentPtr, 0, fontHeight, FIRST_CHAR, CHAR_COUNT, charData)) // !ERROR bitmap size to small
    

    stbtt_PackEnd(&packContext);

   

    for(u16 i = 0; i < CHAR_COUNT; i++)
    {
        stbtt_aligned_quad quad;
        float unused_xpos = 0.0f, unused_ypos = 0.0f;
        stbtt_GetPackedQuad(charData, BITMAP_WIDTH, BITMAP_HEIGHT, i, &unused_xpos, &unused_ypos, &quad, false);
    
        i32 advanceX = 0, leftSideBearing = 0;
        stbtt_GetCodepointHMetrics(font, i + FIRST_CHAR, &advanceX, &leftSideBearing);

        auto& data = m_CharData[i];

        // Texture coordinates
        data.x0 = quad.s0;
        data.x1 = quad.s1;

        data.y0 = quad.t0;
        data.y1 = quad.t1;

        
        data.width  = (quad.x1 - quad.x0);
        data.height = (quad.y1 - quad.y0);
    
        data.baseline = quad.y0; 

        data.ax = advanceX * scale;
        data.lsb = leftSideBearing * scale;

        for(u8 k = 0; k < CHAR_COUNT; k++)
        {
            data.kerning[k] = static_cast<float>(stbtt_GetCodepointKernAdvance(font, i + FIRST_CHAR, k + FIRST_CHAR)) * scale;
        }
    }

    m_Ascent = static_cast<float>(unscaled_ascent) * scale;
    m_Descent = static_cast<float>(unscaled_descent) * scale;
    m_LineHeight =  m_Ascent - m_Descent;
    m_LineGap = static_cast<float>(unscaled_linegap) * scale;

    m_Bitmap = Bitmap::Create(BITMAP_WIDTH, BITMAP_HEIGHT, ImageType::ALPHA);
    m_Bitmap->set_data(bitmap, BITMAP_WIDTH * BITMAP_HEIGHT);

    delete[] bitmap;
};


};