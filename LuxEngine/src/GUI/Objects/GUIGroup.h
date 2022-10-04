#pragma once

#include "GUIChild.h"

#include "GUI/Types.h"


namespace Lux::GUI
{

namespace Group
{

class LayoutElement
{
protected:

	template<typename... Tail>
	LayoutElement(Scale&& scale, LayoutType type, Tail&& ... tail)
		: m_Scale(scale), m_LayoutType(type), m_Elements({ tail... })
	{

	}

	LayoutElement(Scale&& scale, LayoutType type)
		: m_Scale(scale), m_LayoutType(type)
	{}
public:


	void refresh(v3 position, float width, float height)
	{

	}

private:
	Scale m_Scale;
	LayoutType m_LayoutType;

	std::vector<LayoutElement> m_Elements;
};

class Row : public LayoutElement
{
public:
	template<typename... Tail>
	Row(Scale&& scale, Tail&&... tail)
		: LayoutElement(std::forward<Scale>(scale), LayoutType::ROW, tail...)
	{}
};

class Column : public LayoutElement
{
public:
	template<typename... Tail>
	Column(Scale&& scale, Tail&&... tail)
		: LayoutElement(std::forward<Scale>(scale), LayoutType::COLUMN, tail...)
	{}
};

class Element : public LayoutElement
{
public:
	Element(Scale&& scale, GUIChild* id)
		: LayoutElement(std::forward<Scale>(scale), LayoutType::CHILD)
	{
	}
};

}


class GUIGroup : public GUIChild
{
public:
	GUIGroup(
		Group::LayoutElement&& layout, float scale = 1.0f, ScaleType type = ScaleType::FIXED
	)
		:	GUIChild(scale, type, GUIType::GROUP), m_Layout(layout)
	{	
	}

	void refresh_group(v3 position, float width, float height)
	{
		m_Layout.refresh(position, width, height);
	}

private:

	Group::LayoutElement m_Layout;

};

}