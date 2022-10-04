#pragma once

#include "GUIChild.h"

#include "GUI/Types.h"

namespace Lux::GUI
{

namespace Parent
{

class Layout
{

private:


	Container<UUID, LayoutElement*> m_Elements;
	SortedContainer<u32, LayoutElement> m_Layout;
};

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

	SortedContainer<u32, LayoutElement> m_Elements;


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

class GUIParent : public GUIChild
{
protected:

	GUIParent(Parent::LayoutElement&& layout)
		:	GUIChild(1.0f, ScaleType::DYNAMIC, GUIType::PARENT), 
			m_Layout(std::forward<Parent::LayoutElement>(layout))
	{
	}
private:

	Parent::LayoutElement m_Layout;

};

}