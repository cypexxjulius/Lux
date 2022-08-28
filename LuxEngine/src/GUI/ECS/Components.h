#pragma once


#include "Assets/Texture.h"
#include "Assets/Font.h"

#include "Utils/Types.h"

#include "Core/UUID.h"

#include "Registry.h"

#include "GUI/Interface/Interface.h"

#include "GUI/Types.h"

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
	Array<v2, 4> tex_coords;
};

struct TextComponent
{
	float linelength, lineheight;
	float scale;
	Ref<Font> font;
	v2 position;
	v4 color;
	String text;
	List<UUID> glyphs;
};

struct LayoutComponent
{
	std::string name;
	ScaleType scaling_type;
	float scale;

	LayoutSpacing spacing;
	LayoutOrientation orientation;

	float	sum_fixed_scale;
	float	sum_relative_scale;

	UUID parent;

	Set<Pair<u32, UUID>> sections;
};

struct SectionHeaderComponent
{
	UUID title_text;
	UUID retractable_button;

	LayoutComponent layout;
};

struct SectionStyleComponent
{
	u32 child_margin_x, child_margin_y;
	u32 padding_x, padding_y;
	v4 background_color;
};


#define ComponentGroup TypeComponent, TransformComponent, SectionStyleComponent, SectionHeaderComponent, LayoutComponent, TextComponent, GlyphComponent, RectComponent

}