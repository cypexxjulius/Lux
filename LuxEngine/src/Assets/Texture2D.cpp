
#include "Texture2D.h"

#pragma warning(disable: 4505) //Unreferenced formal parameter
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(default: 4505)

#include "Utils/Assert.h"

namespace Lux
{

Texture2D::Texture2D(std::string_view filepath)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, channels; 
    stbi_uc* data = stbi_load(filepath.data(), &width, &height, &channels, 0);

    assert(data);

    switch(channels)
    {
    case 4:
        m_Type = ImageType::RGBA;
        break;

    case 3:
        m_Type = ImageType::RGB;
        break;

    case 1:
        m_Type = ImageType::ALPHA;
        break;

    default: 
        TODO();

    }

    m_Bitmap = Bitmap::create(width, height, m_Type);

    m_Bitmap->set_data(data, width * height * channels);

    stbi_image_free(data);
}

Texture2D::Texture2D(ImageType type, u32 width, u32 height, void* data)
{
    m_Type = type;
    m_Bitmap = Bitmap::create(width, height, m_Type);

    if(data != nullptr)
        m_Bitmap->set_data(data, width * height * static_cast<int>(m_Type));
}

void Texture2D::set_data(void* data)
{
    m_Bitmap->set_data(data, m_Bitmap->width() * m_Bitmap->height() * m_Bitmap->channels());
}

}