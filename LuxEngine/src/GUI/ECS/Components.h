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
	v3 position;
	v3 rotation;
	v3 scale;
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

	void set(ECS::Registry& registry, std::string_view comp_text, const Ref<Font>& comp_font, v3 position, float scale);

	void fit_to_bbox(ECS::Registry& registry, v3 position, v2 max_dimension);

	void recalculate(ECS::Registry& registry,v3 position, float scale);

	void on_destruction(ECS::Registry& registry);
};

struct LayoutInfo
{
	bool fixed = true;
	float scale = 1.0f;
	UUID id;

	LayoutInfo(UUID init_id)
		:	id(init_id)
	{}

	LayoutInfo(bool is_fixed, float layout_scale, UUID section_id)
		:	fixed(is_fixed),
			scale(layout_scale),
			id(section_id)
	{}

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