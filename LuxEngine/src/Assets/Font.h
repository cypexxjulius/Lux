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
	Array<v2, 4> text_coords;
	mat4 transform;
	double advance;

	Glyph(mat4 arg_transform, Array<v2, 4> arg_text_coords, double advance)
		:	text_coords(arg_text_coords),
			transform(arg_transform),
			advance(advance)
	{}
};


class Font {

private:
	friend ResourceManager;

	static void Init();

	static void Shutdown();

private:
	void* m_FontHandle = nullptr;

	Container<char, const Glyph> m_GlyphMetaData;

public:

	Font(const String& file_path);

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

	inline float lineheight()
	{
		return m_LineHeight;
	}

	inline float em_size()
	{
		return m_EmSize;
	}

private:

	float m_LineHeight;
	float m_EmSize;
	float m_AscenderY;
	float m_DecenderY;
	float m_UnderlineThickness;
	float m_UnderlineY;

	Ref<Bitmap> m_Bitmap = nullptr;

	static void* s_FreetypeLibraryHandle;
};

} // namespace Lux