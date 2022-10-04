#pragma once

#include "GUI/ECS/Core.h"

#include "Objects/GUIObject.h"

#include <functional>
#include "Objects/GUIChild.h"


#include "GUIRenderComponents.h"

namespace Lux::GUI
{

class LayoutObject;

class Context
{
private:

	using type_hash = u64;
	
	friend GUIObject;

public:

	Context();
	~Context();


	void render_rects(std::function<void(const Transform&, const Rect&)>);

	void render_glyphs(std::function<void(const Transform&, const Glyph&)>);

	void update_dimensions(float width, float height);

	void refresh();

private:

	void remove_element(UUID id);

	void register_element(UUID id, GUIObject* obj);

	void set_root(UUID id);

	GUIObject& get_object(UUID id);

	const Ref<Font>& get_font()
	{
		return m_Font;
	}

	template<typename T>
	void add_to_render_queue(UUID id, Transform* transform, T* rect)
	{
		TODO();
	}

	template<>
	void add_to_render_queue<Rect>(UUID id, Transform* transform, Rect* rect)
	{
		m_RectComponents.insert({ id, {transform, rect} });
	}

	template<>
	void add_to_render_queue<Glyph>(UUID id, Transform* transform, Glyph* glyph)
	{
		m_GlyphComponents.insert({ id, {transform, glyph} });
	}


	template<typename T>
	void remove_from_render_queue(UUID id)
	{
		TODO();
	}

	template<>
	void remove_from_render_queue<Rect>(UUID id)
	{
		m_RectComponents.erase(id);
	}

	template<>
	void remove_from_render_queue<Glyph>(UUID id)
	{
		m_GlyphComponents.erase(id);
	}

private:

	Container<UUID, GUIObject*> m_GUIElements;

	GUIChild* m_RootElement = nullptr;

	float m_Width = 0.0f, m_Height = 0.0f;

	Container<UUID, Pair<Transform*, Rect*>> m_RectComponents;
	Container<UUID, Pair<Transform*, Glyph*>> m_GlyphComponents;

	Ref<Font> m_Font;
};


}