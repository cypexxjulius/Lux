
#include "Bitmap.h"

#include <glad/glad.h>

#include "Utils/Assert.h"
#include "Utils/Logger.h"

namespace Lux::OpenGL
{

class LuxToGL
{
public:
    static inline GLenum InternalFormat(ImageType type)
    {
        switch (type)
        {
        case ImageType::RGBA:   return GL_RGBA8;
        case ImageType::RGB:    return GL_RGB8;
        case ImageType::ALPHA:  return GL_R8;

        default: TODO();
        }

        return GL_FALSE;
    }

    static inline GLenum DataFormat(ImageType type)
    {
        switch (type)
        {
        case ImageType::RGBA:   return GL_RGBA;
        case ImageType::RGB:    return GL_RGB;
        case ImageType::ALPHA:  return GL_RED;

        default: TODO();
        }

        return GL_FALSE;
    }

    static inline u32 Channels(ImageType type)
    {
        return static_cast<u32>(type);
    }

    static inline GLenum FilterMethod(FilterMethod filter)
    {
        switch (filter)
        {
        case FilterMethod::LINEAR:      return GL_LINEAR;
        case FilterMethod::NEAREST:    return GL_NEAREST;

        default: TODO();
        }

        return GL_FALSE;
    }

    static inline GLenum WrapMethod(WrapMethod method)
    {
        switch (method)
        {
        case WrapMethod::REPEAT:   return GL_REPEAT;
        
        default: TODO();
        }

        return GL_FALSE;
    }

};
    

Bitmap::Bitmap(const BitmapSpec& spec, u32 width, u32 height)
    :   m_Channels(LuxToGL::Channels(spec.type)),
        m_Width(width),
        m_Height(height),
        m_Spec(spec)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

    glTextureStorage2D(m_ID, 1, LuxToGL::InternalFormat(m_Spec.type), width, height);

    glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, LuxToGL::FilterMethod(m_Spec.min_filter));
    glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, LuxToGL::FilterMethod(m_Spec.mag_filter));

    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, LuxToGL::WrapMethod(m_Spec.wrap_s));
    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, LuxToGL::WrapMethod(m_Spec.wrap_t));
}

void Bitmap::set_data(const void* data, u32 size)
{
    assert(size == (m_Width * m_Height * m_Channels));
    if(m_Spec.unpack_aligned)
        glPixelStorei(GL_UNPACK_ALIGNMENT, GL_TRUE);
    glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, LuxToGL::DataFormat(m_Spec.type), GL_UNSIGNED_BYTE, data);
}


Bitmap::~Bitmap()
{
    glDeleteTextures(1, &m_ID);
}

void Bitmap::bind(u8 slot)
{
    glBindTextureUnit(slot, m_ID);
}

}