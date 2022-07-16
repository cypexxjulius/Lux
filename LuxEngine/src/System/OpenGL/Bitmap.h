#pragma once 

#include "Graphics/Core/Bitmap.h"
#include "Utils/Types.h"

namespace Lux::OpenGL
{

class Bitmap final : public ::Lux::Bitmap
{
private:
    u8 m_Channels, m_BoundSlot = 0;
    DataFormat m_DataFormat;
    u32 m_ID;
    u32 m_Width, m_Height;

private:

    void CreateEmptyImpl(u32 width, u32 height, InternalFormat internalFormat, DataFormat dataFormat);

public:

    Bitmap(u32 width, u32 height, ImageType type);

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

    virtual DataFormat data_format() const override
    { return m_DataFormat; }

};


} 