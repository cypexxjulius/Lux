#pragma once

#include "GUIObject.h"

#include "GUI/GUIRenderComponents.h"

namespace Lux::GUI
{

class RectObject : GUIObject
{
public:
	RectObject()
		: GUIObject(GUIType::RECT)
	{
		register_rect(&m_Transform, &m_RectComponent);
	}

	RectObject(const v4& color)
		: GUIObject(GUIType::RECT)
	{
		m_RectComponent.color = color;
		register_rect(&m_Transform, &m_RectComponent);
	}

	~RectObject()
	{
		unregister_rect();
	}

	void set_position(const v3& position)
	{
		m_Transform.position = position;
	}

	void set_scale(const v3& scale)
	{
		m_Transform.scale = scale;
	}

	void set_color(const v4& color)
	{
		m_RectComponent.color = color;
	}

	void refresh(v3 position, float width, float height)
	{
		m_Transform.position = position;
		m_Transform.scale = { width, height, 1.0f };
	}

	Rect m_RectComponent {};
	TransformComponent m_Transform {};

};

}