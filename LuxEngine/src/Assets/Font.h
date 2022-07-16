#pragma once

#include <memory>
#include <string_view>

#include "Graphics/Core/Bitmap.h"
#include "Utils/Assert.h"
#include "Utils/Types.h"

namespace Lux {

class ResourceManager;

struct Glyph
{
	std::array<v2, 4> text_coords;
	mat4 transform;
	double advance;

	Glyph(mat4 arg_transform, std::array<v2, 4> arg_text_coords, double advance)
		:	text_coords(arg_text_coords),
			transform(arg_transform),
			advance(advance)
	{}
};


class Font {

private:
	friend ResourceManager;

	Ref<Bitmap> m_Bitmap = nullptr;

	static void* s_FreetypeLibraryHandle;

	static void Init();

	static void Shutdown();

private:
	void *m_FontHandle = nullptr;
	
	std::unordered_map<char, const Glyph> m_GlyphMetaData;

public:

	Font(const std::string &file_path);

	~Font();

	inline void bind(u8 slot) const { m_Bitmap->bind(slot); };

	inline const Glyph& getGlyph(char character)
	{
		auto it = m_GlyphMetaData.find(character);

		Verify(it != m_GlyphMetaData.end());

		return it->second;
	}

	inline Ref<Bitmap>& bitmap()
	{
		return m_Bitmap;
	}


};

} // namespace Lux