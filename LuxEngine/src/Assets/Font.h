#pragma once

#include <memory>
#include <string_view>

#include "Graphics/Bitmap.h"
#include "Utils/Assert.h"
#include "Utils/Types.h"

namespace Lux {

class ResourceManager;

struct Glyph
{
	std::array<double, 4> text_coords;
	std::array<double, 4> positions;
	double advance;

	Glyph(std::array<double, 4> arg_text_coords, std::array<double, 4> arg_positions, double advance)
		:	text_coords(arg_text_coords),
			positions(arg_positions),
			advance(advance)
	{}
};


class Font {

private:
	friend ResourceManager;

	Scope<Bitmap> m_Bitmap = nullptr;

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

};

} // namespace Lux