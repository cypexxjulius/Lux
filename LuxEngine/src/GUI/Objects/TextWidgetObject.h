#include "WidgetObject.h"

#include "TextObject.h"
#include "GUIGroup.h"

namespace Lux::GUI
{


class TextWidgetObject : public GUIGroup
{
private:

	static constexpr float height = 40.0f;

public:

TextWidgetObject(const std::string& string, const Ref<Font>& font, float scale)
		: GUIGroup(Element(100.0_rel, &m_TextObj), scale * height, ScaleType::FIXED),
		m_Shortened(false),
		m_MaxWidth(0.0f),
		m_Scale(scale * height),
		m_Text(string),
		m_TextObj(string, font, 1.0f)
{	

	
	m_TextObj.set_color({ 1.0f, 1.0f,1.0f,1.0f });

	m_MaxWidth = m_TextObj.get_width();
}


virtual void refresh(v3 position, float width, float height) override
{
	if (m_MaxWidth > width)
	{
		m_TextObj.replace_ending("...", width);
		m_Shortened = true;
	}

	if (width >= m_MaxWidth && m_Shortened)
		m_TextObj.set_text(m_Text);
	
	m_TextObj.refresh(position, width, height);
}


void set_text(const std::string& text)
{
	m_Text = text;
}
private:

	bool m_Shortened;
	float m_MaxWidth;
	float m_Scale;
	std::string m_Text;

	TextObject m_TextObj;
};

}