#pragma once

#include "GUI/ECS/Core.h"

#include "Objects/GUIObject.h"

#include <functional>


namespace Lux::GUI
{


class Context
{
private:

	using type_hash = u64;
	
	friend GUIObject;

public:

	Context();
	~Context();


	void render_rects(std::function<void(const TransformComponent&, const RectComponent&)>);

	void render_glyphs(std::function<void(const TransformComponent&, const GlyphComponent&)>);

	void update_dimensions(float width, float height);

	void refresh();

private:

	void remove_element(UUID id);

	void register_element(UUID id, GUIObject* obj);

	void set_root(GUIObject* id);

	GUIObject& get_object(UUID id);

	const Ref<Font>& get_font()
	{
		return m_Font;
	}

private:

	Container<UUID, GUIObject*> m_GUIElements;

	GUIObject *m_RootElement = nullptr;

	float m_Width = 0.0f, m_Height = 0.0f;

	ECS::Registry m_Registry;

	Ref<Font> m_Font;
};


}