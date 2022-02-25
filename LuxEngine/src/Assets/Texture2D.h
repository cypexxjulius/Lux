#pragma once

#include "Renderer/Bitmap.h"

#include <string_view>

namespace Lux
{

class Texture2D
{
private:
    Bitmap* m_Bitmap = nullptr;
    ImageType m_Type;

public:

    Texture2D(std::string_view filepath);

    Texture2D() {}

    Texture2D(Texture2D& temp)
    : m_Bitmap(std::move(temp.m_Bitmap))
    , m_Type(temp.m_Type)
    {
        temp.m_Bitmap = nullptr;
    }


    Texture2D(ImageType type, u32 width, u32 height, void* data);

    ~Texture2D()
    { 
        if(valid())
        {
            delete m_Bitmap;
            m_Bitmap = nullptr;
        }     
    }  

    inline void bind(u8 slot)
    { m_Bitmap->bind(slot); }

    inline u32 width() const
    { return m_Bitmap->width(); }

    inline u32 height() const
    { return m_Bitmap->height(); }

    inline u32 id() const 
    { return m_Bitmap->id(); }

    constexpr ImageType type() const
    { return m_Type; }

    inline bool valid() const
    { return m_Bitmap != nullptr; }

    
    void set_data(void* data); 

};

}