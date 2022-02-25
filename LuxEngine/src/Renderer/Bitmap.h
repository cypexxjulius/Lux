#pragma once 

#include "Utils/Types.h"

namespace Lux
{

enum class InternalFormat : u8
{
    RGBA8,
    RGB8,
    RED8
};

enum class DataFormat : u8 
{
    RGBA,
    RGB,
    RED
};

enum class ImageType : u8
{
    ALPHA = 1,
    RGB = 3,
    RGBA = 4,
};

class Bitmap
{

protected:

    Bitmap() = default;

public:

    virtual ~Bitmap() {}

    virtual void bind(u8 slot) = 0;

    virtual void set_data(void *data, u32 size) = 0;

    virtual u32 width() const = 0;

    virtual u32 height() const = 0;

    virtual u8 channels() const = 0;

    virtual u32 id() const = 0;

    virtual DataFormat data_format() const = 0;

    static Bitmap* create(u32 width, u32 height, ImageType type); 

};

}