#pragma once

#include "GUIChild.h"

#include "GlyphObject.h"

namespace Lux::GUI
{

class TextObject final : public GUIChild 
{
public:

	TextObject(const std::string& string, const Ref<Font>& font, float scale)
		:	GUIChild(1.0f, ScaleType::RELATIVE, GUIType::CHILD)
	{
		m_Text = string;
		m_Font = font;
		m_Scale = scale;
		m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	void set_text(std::string_view new_text)
	{
		if(new_text == m_Text)
			return;

		m_Text = new_text;
	}

	void set_position(const v3& position)
	{
		m_Position = position;
	}

	void set_font(const Ref<Font>& font)
	{
		if(m_Font == font)
			return;

		m_Font = font;
	}
	
	void set_color(const v4& color)
	{
		
		m_Color = color;
		for(auto id : m_Glyphs)
			GetObject<GlyphObject>(id).get_glyph_component().color = color;
	}

	void fit_to_bbox(float width, float height)
	{
		if(!m_Font)
			return;

		float scale = height / m_Font->lineheight();

		if(m_Linelength)
		{
			float width_scale = width / (m_Linelength / m_Scale);


			scale = std::min(width_scale, scale);
		}

		m_Scale = scale;
	}

	void refresh(v3 position, float width, float height) override
	{
		m_Position = position;
		
		if(height > m_Font->lineheight() || width > m_Linelength)
			fit_to_bbox(width, height);
		
		recalculate();
	}


	void recalculate()
	{
		u32 actual_char_count = 0;
		for (auto character : m_Text)
			actual_char_count += (character != ' ');

		if (actual_char_count < old_char_count)
		{
			u32 diff = old_char_count - actual_char_count;
			for (u32 i = 0; i < diff; i++)
			{
				UUID id = m_Glyphs.back();
				delete &GetObject(id);
				m_Glyphs.pop_back();
			}
		}
		else if (actual_char_count > old_char_count)
		{
			u32 diff = actual_char_count - old_char_count;
			for (u32 i = 0; i < diff; i++)
			{
				m_Glyphs.emplace_back((new GlyphObject)->get_id());
			}
			
		}

		m_Linelength = 0.0;
		float scale = m_Scale;

		float lineheight = m_Font->lineheight() * scale;
		v3 position = m_Position;

		position.y += m_Font->descender() * scale;


		u32 index = 0;
		for (auto character : m_Text)
		{
			auto& glyph = m_Font->getGlyph(character);

			if (character != ' ')
			{
				UUID glyph_id = m_Glyphs[index++];

				GlyphObject& glyph_obj = GetObject<GlyphObject>(glyph_id);
				
				Glyph& glyph_component = glyph_obj.get_glyph_component();



				glyph_component.color = m_Color;
				glyph_component.font = m_Font;
				glyph_component.tex_coords = glyph.text_coords;
				glyph_component.glyph = character;
				glyph_component.char_transform = glyph.transform;

				TransformComponent& transform = glyph_obj.get_transform_component();
				
				transform.position = position;
				transform.scale = { scale, scale, 1.0f };
			}

			m_Linelength += (float)glyph.advance * scale;
			position.x += (float)glyph.advance * scale;
		
		}
	}

	void replace_ending(const std::string& new_ending, float max_width)
	{
		float ending_width = 0.0f;
		for (char character : new_ending)
		{
			auto& glyph = m_Font->getGlyph(character);
			ending_width += (float)glyph.advance * m_Scale;
		}

		u32 index = 0;
		for (char character : m_Text)
		{
			auto& glyph = m_Font->getGlyph(character);
			max_width -= (float)glyph.advance * m_Scale;

			if (max_width <= 0)
				break;

			index++;
		}
		
		m_Text = m_Text.substr(index) + new_ending;
		
		m_Linelength = get_width();

		Verify(m_Linelength <= max_width);
	}


	float get_width()
	{
		float linelength = 0.0f;
		
		for (char character : m_Text)
		{
			auto& glyph = m_Font->getGlyph(character);
			linelength += (float)glyph.advance * m_Scale;
		}
		
		return linelength;
	}

	float m_Linelength;
	float m_Scale;
	u32 old_char_count;
	Ref<Font>	m_Font;
	List<UUID>	m_Glyphs;
	v3		m_Position;
	v4		m_Color;
	String		m_Text;

	
};

}