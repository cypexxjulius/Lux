#pragma once


#include "Assets/Texture.h"
#include "Assets/Font.h"

#include "Utils/Types.h"

#include "Core/UUID.h"

#include "Registry.h"

#include "GUI/Interface/Interface.h"

#include "GUI/ElementTypes.h"

namespace Lux::GUI
{

struct TransformComponent
{
	v3 position;
	v3 rotation;
	v3 scale;
};

struct TypeComponent
{
	ElementType type;
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
	std::array<v2, 4> tex_coords;
	Ref<Font> font;
};

struct TextComponent
{
	Ref<Font> font;
	std::string text;
	v4 color;
	v2 position;
	float linelength, lineheight;

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

	LayoutOrientation orientation = LayoutOrientation::VERTICAL;
	LayoutSpacing spacing = LayoutSpacing::START;

	float scale = 1.0f;
	u32 max_scale_compound = 0;

	UUID parent;

	std::vector<LayoutInfo> sections;
};


}