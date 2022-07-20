#pragma once 

#include "Utils/Types.h"
#include <memory>

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
    NONE = 0,
    ALPHA = 1,
    RGB = 3,
    RGBA = 4,
};

enum class FilterMethod
{
    NONE,
    LINEAR,
    NEAREST
};

enum class WrapMethod
{
    NONE,
    REPEAT
};

struct BitmapSpec
{
    ImageType type = ImageType::NONE;
    bool unpack_aligned = false;

    FilterMethod min_filter = FilterMethod::LINEAR;
    FilterMethod mag_filter = FilterMethod::NEAREST;
    WrapMethod wrap_s = WrapMethod::REPEAT;
    WrapMethod wrap_t = WrapMethod::REPEAT;
};

class Bitmap
{

protected:

    Bitmap() = default;

public:

    virtual ~Bitmap() {}

    virtual void bind(u8 slot) = 0;

    virtual void set_data(const void *data, u32 size) = 0;

    virtual u32 width() const = 0;

    virtual u32 height() const = 0;

    virtual u8 channels() const = 0;

    virtual u32 id() const = 0;

    virtual const BitmapSpec& spec() const = 0;

    static std::shared_ptr<Bitmap> Create(const BitmapSpec& spec, u32 width, u32 height); 

};

}