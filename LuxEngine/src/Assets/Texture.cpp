
#include "Texture.h"


#pragma warning(disable: 4505) // Unreferenced formal parameter
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(default: 4505)

#include "Utils/Assert.h"

#include <string_view>


namespace Lux
{

Texture::Texture(std::string_view filepath)
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, channels; 
    stbi_uc* data = stbi_load(filepath.data(), &width, &height, &channels, 0);

    Verify(data);

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

    BitmapSpec spec;

    spec.type = m_Type;

    m_Bitmap = Bitmap::Create(spec, width, height);

    m_Bitmap->set_data(data, width * height * channels);

    stbi_image_free(data);
}

Texture::Texture(ImageType type, u32 width, u32 height, void* data)
{
    m_Type = type;

    BitmapSpec spec;

    spec.type = m_Type;

    m_Bitmap = Bitmap::Create(spec, width, height);

    if(data != nullptr)
        m_Bitmap->set_data(data, width * height * static_cast<int>(m_Type));
}

void Texture::set_data(void* data)
{
    m_Bitmap->set_data(data, m_Bitmap->width() * m_Bitmap->height() * m_Bitmap->channels());
}

}