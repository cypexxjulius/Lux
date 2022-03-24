#pragma once

#include <memory>
#include <string_view>

#include "Graphics/Bitmap.h"
#include "Resources/Manager.h"

namespace Lux
{

class Texture2D
{
private:
    friend ResourceManager;

    std::unique_ptr<Bitmap> m_Bitmap;
    ImageType m_Type;

    Texture2D(std::string_view filepath);
public:

    Texture2D(ImageType type, u32 width, u32 height, void* data); 

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