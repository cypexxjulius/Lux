
#include "Bitmap.h"

#include "System/OpenGL/Bitmap.h"

namespace Lux
{


std::unique_ptr<Bitmap> Bitmap::Create(u32 width, u32 height, ImageType type)
{
    return std::make_unique<OpenGL::Bitmap>(width, height, type);
}

}