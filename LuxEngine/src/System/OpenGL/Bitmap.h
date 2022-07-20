#pragma once 

#include "Graphics/Core/Bitmap.h"
#include "Utils/Types.h"

namespace Lux::OpenGL
{

class Bitmap final : public ::Lux::Bitmap
{
private:
    u32 m_ID = 0;
    u8 m_Channels;
    u32 m_Width;
    u32 m_Height;

    BitmapSpec m_Spec;

public:

    Bitmap(const BitmapSpec& spec, u32 width, u32 height);

    ~Bitmap();

    virtual void bind(u8 slot) override;

    virtual void set_data(const void* data, u32 size) override;

    virtual u32 width() const override
    { return m_Width; }

    virtual u32 height() const override 
    { return m_Height; }

    virtual u8 channels() const override 
    { return m_Channels; }

    virtual u32 id() const override 
    { return m_ID; }

    virtual const BitmapSpec& spec() const override
    { return m_Spec; }

};


} 