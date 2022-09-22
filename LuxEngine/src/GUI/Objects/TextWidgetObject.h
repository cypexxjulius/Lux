#include "WidgetObject.h"

#include "TextObject.h"

#include "GUI/ECS/Components.h"

namespace Lux::GUI
{

class TextWidgetObject : public WidgetObject
{
private:

	static constexpr float height = 40.0f;

public:

TextWidgetObject(const std::string& string, const Ref<Font>& font, float scale)
	: WidgetObject(TypeComponent::TEXT_WIDGET)
{

	auto& text = attach_component<TextWidgetComponent>();
	text.text = string;
	text.font = font;
	text.scale = scale * height;
	text.shortened = false;
	text.text_obj = CreateObject<TextObject>(string, font, scale);

	auto& layout = get<LayoutComponent>();

	layout.scale = height * scale;
	layout.scaling_type = ScaleType::FIXED;
	

	auto& text_obj = GetObject<TextObject>(text.text_obj);

	text_obj.set_color({ 1.0f, 1.0f,1.0f,1.0f });

	text.max_width = text_obj.get_width();
}


virtual void refresh(v2 position, float width, float height, float depth) override
{
	auto& text = get<TextWidgetComponent>();

	auto& text_obj = GetObject<TextObject>(text.text_obj);

	if (text.max_width > width)
	{
		text_obj.replace_ending("...", width);
		text.shortened = true;
	}

	if (width >= text.max_width && text.shortened)
		text_obj.set_text(text.text);
	
	text_obj.refresh(position, width, height, depth);
}


void set_text(const std::string& text)
{
	auto& text_component = attach_component<TextWidgetComponent>();
	text_component.text = text;
}

};

}