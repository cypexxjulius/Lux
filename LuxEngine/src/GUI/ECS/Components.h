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
	TEXT,
	RECT,
	SECTION_TOP,
	SECTION_BOTTOM,
};


struct RectComponent
{
	float tiling;
	Ref<Texture> texture;
	v4 color;
};

struct GlyphComponent
{
	char glyph;
	v4 color;
	Ref<Font> font;
	Array<v2, 4> tex_coords;
	mat4 char_transform;
};

struct TextComponent
{

	float linelength;
	float scale;
	Ref<Font> font;
	v3 position;
	String text;
	List<UUID> glyphs;
	v4 color;

};

struct LayoutComponent
{
	ScaleType scaling_type;
	float scale;

	
	v2 child_margin;
	v2 padding;

	LayoutSpacing spacing;

	float	sum_fixed_scale;
	float	sum_relative_scale;

	UUID parent;

	SortedContainer<u32, UUID> sections;
};

struct SectionStyleComponent
{
	float top_section_height;
	float bottom_section_height;
	
	float outline_width;

	v4 top_section_bg_color;
	v4 bottom_section_bg_color;
	v4 background_color;
	v4 outline_color;

	v4 title_color;
};

struct SectionComponent
{
	std::string name;

	UUID outline;
	UUID background;
	UUID top_section;
	UUID bottom_section;

	v2 position;
	float width, height, depth;
};


template<typename... Types>
struct ComponentGroup{};

ComponentGroup <
	TypeComponent, 
	SectionComponent, 
	TransformComponent, 
	LayoutComponent, 
	TextComponent, 
	GlyphComponent, 
	RectComponent
> Components{};
}