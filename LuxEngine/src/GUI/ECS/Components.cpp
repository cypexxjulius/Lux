#include "Components.h"


namespace Lux::GUI
{

void TextComponent::set(ECS::Registry& registry, std::string_view comp_text, const Ref<Font>& comp_font, v3 position, float scale)
{
	text = comp_text;
	font = comp_font;
	recalculate(registry, position, scale);
}

void TextComponent::fit_to_bbox(ECS::Registry& registry, v3 position, v2 max_dimension)
{
	Verify(linelength != 0);
	float width_scale = max_dimension.x / linelength;
	float height_scale = max_dimension.y / lineheight;

	float scale = std::min(width_scale, height_scale);
	recalculate(registry, position, scale);
}

	
void TextComponent::recalculate(ECS::Registry& registry,v3 position, float scale)
{
	glyphs.reserve(text.size());
	linelength = 0.0;
	lineheight = font->lineheight() * scale;

	for (auto character : text)
	{
		auto& glyph = font->getGlyph(character);

		if (character != ' ')
		{
			glyphs.emplace_back(registry.create());

			auto& element = glyphs.back();
			registry.add_component<TransformComponent>(element) = { v3{ 1.0f, 1.0f, 1.0f }, position, v3{scale, scale, 1.0f} };
			registry.add_component<GlyphComponent>(element) = { color, glyph.text_coords, font };
		}

		linelength += (float)glyph.advance;
	}
}

void TextComponent::on_destruction(ECS::Registry& registry)
{
	for(auto& section : glyphs )
		registry.destroy(section);
}

}