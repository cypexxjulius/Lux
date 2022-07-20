
#include "Bitmap.h"

#include "System/OpenGL/Bitmap.h"

namespace Lux
{


std::shared_ptr<Bitmap> Bitmap::Create(const BitmapSpec& spec, u32 width, u32 height)
{
    return std::make_shared<OpenGL::Bitmap>(spec, width, height);
}

}