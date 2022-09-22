#pragma once

#include "GUIObject.h"

#include "GlyphObject.h"

namespace Lux::GUI
{

class TextObject final : public GUIObject 
{
public:

	TextObject(const std::string& string, const Ref<Font>& font, float scale)
		:	GUIObject(TypeComponent::TEXT)
	{
		auto& text = attach_component<TextComponent>();
		text.text = string;
		text.font = font;
		text.scale = scale;

	}

	void set_text(std::string_view comp_text)
	{
		auto& text = get<TextComponent>();
		
		if(comp_text == text.text)
			return;

		text.text = comp_text;
	}

	void set_position(const v3& position)
	{
		auto& text = get<TextComponent>();

		text.position = position;
	}

	void set_font(const Ref<Font>& font)
	{
		auto& text = get<TextComponent>();
		
		if(font == text.font)
			return;

		text.font = font;
	}
	
	void set_color(const v4& color)
	{
		auto& text = get<TextComponent>();

		text.color = color;
		for(auto id : text.glyphs)
			Get<GlyphComponent>(id).color = text.color;
	}

	void fit_to_bbox(float width, float height)
	{
		auto& text = get<TextComponent>();
		if(!text.font)
			return;

		float scale = height / text.font->lineheight();
		if(text.linelength)
		{
			float width_scale = width / (text.linelength / text.scale);


			scale = std::min(width_scale, scale);
		}

		text.scale = scale;
	}

	virtual void refresh(v2 position, float width, float height, float depth) override
	{
		auto& text = get<TextComponent>();

		text.position = { position, depth };
		if(height > text.font->lineheight() || width > text.linelength)
			fit_to_bbox(width, height);
		
		recalculate();
	}


	void recalculate()
	{
		auto& text = get<TextComponent>();

		u32 actual_char_count = 0;
		for (auto character : text.text)
			actual_char_count += (character != ' ');

		text.glyphs.reserve(actual_char_count);
		while (text.glyphs.size() < actual_char_count)
		{
			text.glyphs.emplace_back(CreateObject<GlyphObject>());
		}

		while (text.glyphs.size() > actual_char_count)
		{
			delete& GetObject(text.glyphs.back());
			text.glyphs.pop_back();
		}


		text.linelength = 0.0;

		float lineheight = text.font->lineheight() * text.scale;
		v3 position = text.position;

		position.y += text.font->descender() * text.scale;

		float scale = text.scale;

		u32 index = 0;
		for (auto character : text.text)
		{
			auto& glyph = text.font->getGlyph(character);

			if (character != ' ')
			{
				UUID glyph_id = text.glyphs[index++];

				GlyphObject& glyph_obj = GetObject<GlyphObject>(glyph_id);
				
				GlyphComponent& glyph_component = glyph_obj.get_glyph_component();



				glyph_component.color = text.color;
				glyph_component.font = text.font;
				glyph_component.tex_coords = glyph.text_coords;
				glyph_component.glyph = character;
				glyph_component.char_transform = glyph.transform;

				TransformComponent& transform = glyph_obj.get_transform_component();
				
				transform.position = position;
				transform.scale = { scale, scale, 1.0f };
			}

			text.linelength += (float)glyph.advance * scale;
			position.x += (float)glyph.advance * scale;
		
		}
	}

	void destroy(UUID id)
	{
		auto& text = get<TextComponent>();

		for (auto& glyph : text.glyphs)
			DestroyPlain(glyph);
	}

	void replace_ending(const std::string& new_ending, float max_width)
	{
		auto& text = get<TextComponent>();
		float ending_width = 0.0f;
		for (char character : new_ending)
		{
			auto& glyph = text.font->getGlyph(character);
			ending_width += (float)glyph.advance * text.scale;
		}

		u32 index = 0;
		for (char character : text.text)
		{
			auto& glyph = text.font->getGlyph(character);
			max_width -= (float)glyph.advance * text.scale;

			if (max_width <= 0)
				break;

			index++;
		}
		
		text.text = text.text.substr(index) + new_ending;
	}


	float get_width()
	{
		auto& text = get<TextComponent>();

		float linelength = 0.0f;
		
		for (char character : text.text)
		{
			auto& glyph = text.font->getGlyph(character);
			linelength += (float)glyph.advance * text.scale;
		}
		
		return linelength;
	}
	
};

}