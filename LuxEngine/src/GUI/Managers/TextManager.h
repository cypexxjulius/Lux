#pragma once

#include "Manager.h"

namespace Lux::GUI
{

class TextManager final : public Manager 
{
public:

	virtual void on_shutdown() {}

	void set_text(UUID id, std::string_view comp_text, const Ref<Font>& comp_font, v3 position, float scale)
	{
		auto& glyph = get_component<TextComponent>(id);

		glyph.text = comp_text;
		glyph.font = comp_font;
		
		recalculate(id, position, scale);
	}

	void fit_to_bbox(UUID id, v3 position, v2 max_dimension)
	{
		auto& text = get_component<TextComponent>(id);
		
		Verify(text.linelength != 0);
		float width_scale = max_dimension.x / text.linelength;
		float height_scale = max_dimension.y / text.lineheight;

		float scale = std::min(width_scale, height_scale);
		recalculate(id, position, scale);
	}


	void recalculate(UUID id, v3 position, float scale)
	{
		auto& text = get_component<TextComponent>(id);

		text.glyphs.reserve(text.text.size());
		text.linelength = 0.0;
		text.lineheight = text.font->lineheight() * scale;

		for (auto character : text.text)
		{
			auto& glyph = text.font->getGlyph(character);

			if (character != ' ')
			{
				text.glyphs.emplace_back(create_plain());

				auto& element = text.glyphs.back();
				add_component<TransformComponent>(element) = { v3{ 1.0f, 1.0f, 1.0f }, position, v3{scale, scale, 1.0f} };
				auto& glyph_component = add_component<GlyphComponent>(element);
				glyph_component.color = text.color;
				glyph_component.font = text.font;
				glyph_component.tex_coords = glyph.text_coords;
			}

			text.linelength += (float)glyph.advance;
		}
	}

	void destroy(UUID id)
	{
		auto& text = get_component<TextComponent>(id);

		for (auto& glyph : text.glyphs)
			destroy(glyph);
	}
	
};

}