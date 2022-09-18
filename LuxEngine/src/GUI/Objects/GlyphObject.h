#pragma once


#include "GUIObject.h"

namespace Lux::GUI
{

class GlyphObject final : public GUIObject
{
public:
	GlyphObject()
		:	GUIObject(TypeComponent::GLYPH)
	{
		attach_component<GlyphComponent>();
		attach_component<TransformComponent>();
	}

	virtual void refresh(v2 position, float width, float height, float depth) override {};

	GlyphComponent& get_glyph_component()
	{
		return get<GlyphComponent>();
	}

	TransformComponent& get_transform_component()
	{
		return get<TransformComponent>();
	}


};


}