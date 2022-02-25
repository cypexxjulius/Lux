
#include "Bitmap.h"

#include <glad/glad.h>

#include "Utils/Assert.h"
#include "Utils/Logger.h"

namespace Lux::OpenGL
{
    
static inline GLenum to_gl_internal_format(InternalFormat format)
{
    switch(format)
    {
        case InternalFormat::RGBA8 : return GL_RGBA8;
        case InternalFormat::RGB8 :return GL_RGB8;  
        case InternalFormat::RED8 : return GL_R8;

        default: TODO();
    }

    return GL_FALSE;
}

static inline GLenum to_gl_data_format(DataFormat format)
{
    switch(format)
    {
        case DataFormat::RGBA : return GL_RGBA;
        case DataFormat::RGB : return GL_RGB;  
        case DataFormat::RED : return GL_RED;

        default: TODO();

    }

    return GL_FALSE;
}


void Bitmap::CreateEmptyImpl(u32 width, u32 height, InternalFormat internalFormat, DataFormat dataFormat)
{
    m_DataFormat = dataFormat;

    m_Width = width;
    m_Height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, 1, to_gl_internal_format(internalFormat), width, height);

	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

Bitmap::Bitmap(u32 width, u32 height, ImageType type)
{
    switch (type)
    {
    case (ImageType::RGBA):
        CreateEmptyImpl(width, height, InternalFormat::RGBA8, DataFormat::RGBA);
        m_Channels = 4;
        return;

    case (ImageType::RGB):
        CreateEmptyImpl(width, height, InternalFormat::RGB8, DataFormat::RGB);
        m_Channels = 3;
        return;

    case (ImageType::ALPHA):
        CreateEmptyImpl(width, height, InternalFormat::RED8, DataFormat::RED);
        m_Channels = 1;
        return;
    
    default:
        TODO();
        return;

    }
}

void Bitmap::set_data(void* data, u32 size)
{
    Assert(size == (m_Width * m_Height * m_Channels), "Invalid buffer size specified");
    glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, to_gl_data_format(m_DataFormat), GL_UNSIGNED_BYTE, data);
}


Bitmap::~Bitmap()
{
    glDeleteTextures(1, &m_ID);
}

void Bitmap::bind(u8 slot)
{
    glBindTextureUnit(slot, m_ID);
    m_BoundSlot = slot;
}

}