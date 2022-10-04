#pragma once

#include "GUIObject.h"

namespace Lux::GUI
{

class RectObject final : public GUIObject
{
public:
	RectObject()
		: GUIObject(TypeComponent::RECT)
	{
		attach_component<TransformComponent>();
		attach_component<RectComponent>();
	}

	RectObject(const v4& color)
		: GUIObject(TypeComponent::RECT)
	{
		attach_component<TransformComponent>();
		attach_component<RectComponent>().color = color;
	}


	void set_position(const v3& position)
	{
		auto& transform = get<TransformComponent>();

		transform.position = position;
	}

	void set_scale(const v3& scale)
	{
		auto& transform = get<TransformComponent>();

		transform.scale = scale;
	}

	void set_color(const v4& color)
	{
		auto& rect = get<RectComponent>();
		rect.color = color;
	}

	virtual void refresh(v2 position, float width, float height, float depth) override
	{
		auto& transform = get<TransformComponent>();

		transform.position = { position, depth };
		transform.scale = { width, height, 1.0f };
	}

};

}