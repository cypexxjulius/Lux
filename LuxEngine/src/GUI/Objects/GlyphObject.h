#pragma once


#include "GUIObject.h"

#include "GUI/GUIRenderComponents.h"

namespace Lux::GUI
{



class GlyphObject final : public GUIObject
{
public:
	GlyphObject()
		:	GUIObject(GUIType::GLYPH)
	{
		register_glyph(&m_TransformComponent, &m_GlyphComponent);
	}

	~GlyphObject()
	{
		unregister_glyph();
	}

	Glyph& get_glyph_component()
	{
		return m_GlyphComponent;
	}

	Transform& get_transform_component()
	{
		return m_TransformComponent;
	}

private:

	Glyph m_GlyphComponent;

	Transform m_TransformComponent;


};


}