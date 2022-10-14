#pragma once

#include "GUIObject.h"

#include "GUI/Types.h"

namespace Lux::GUI
{

class GUIChild : public GUIObject
{
public:
	GUIChild(float scale, ScaleType scale_type, GUIType type = GUIType::CHILD)
		:	GUIObject(type), m_Scale(scale), m_ScaleType(scale_type)
	{
	}

	virtual void refresh(v3 position, float width, float height) = 0;


	inline float get_scale()
	{
		return m_Scale;
	}

	inline ScaleType get_scale_type()
	{
		return m_ScaleType;
	}

	inline const v2& get_margin()
	{
		return m_Margin;
	}

	inline const v2& get_padding()
	{
		return m_Padding;
	}

	inline const v2& get_border()
	{
		return m_Border;
	}

	inline void set_margin(const v2& margin)
	{
		m_Margin = margin;
	}

	inline void set_padding(const v2& padding)
	{
		m_Padding = padding;
	}

	inline void set_border(const v2& border)
	{
		m_Border = border;
	}

private:

	float m_Scale;
	ScaleType m_ScaleType;

	v2 m_Margin { 0.0f, 0.0f };
	v2 m_Padding { 0.0f, 0.0f };
	v2 m_Border { 0.0f, 0.0f };
};

}