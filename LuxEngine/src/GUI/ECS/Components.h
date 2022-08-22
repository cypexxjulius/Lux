#pragma once


#include "Assets/Texture.h"
#include "Assets/Font.h"

#include "Utils/Types.h"

#include "Core/UUID.h"

#include "Registry.h"

#include "GUI/Interface/Interface.h"

namespace Lux::GUI
{

struct TransformComponent
{
	v3 position { 0 };
	v3 rotation { 0 };
	v3 scale { 0 };
};

enum TypeComponent
{
	SECTION,
};


struct RectComponent
{
	float tiling;
	Ref<Texture> texture;
	v4 color;
};

struct GlyphComponent
{
	v4 color;
	Ref<Font> font;
	std::array<v2, 4> tex_coords;
};

struct TextComponent
{
	Ref<Font> font;
	std::string text;
	v4 color;
	v2 position;
	float linelength, lineheight;
	float scale;

	std::vector<UUID> glyphs;
};

struct LayoutInfo
{
	bool fixed = true;
	float scale = 1.0f;
	UUID id = 0;


	bool operator==(LayoutInfo& other)
	{
		return other.id == id;
	}
};

struct LayoutComponent
{
	bool fixed;

	LayoutSpacing spacing = LayoutSpacing::START;
	LayoutOrientation orientation = LayoutOrientation::VERTICAL;

	float scale = 1.0f;
	u32 max_scale_compound = 0;

	UUID parent = 0;

	std::vector<LayoutInfo> sections;
};

struct SectionHeaderComponent
{
	UUID title_text;
	UUID retractable_button;
	LayoutComponent layout;
};


}