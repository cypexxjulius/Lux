#pragma once

#include "Utils/Types.h"

#include "Types.h"

#include "Core/UUID.h"

#include "Utils/Assert.h"

namespace Lux::GUI
{

struct LayoutStructure
{
	Scale m_Scale;
	LayoutStructureType m_LayoutType;
	UUID m_ID;

	LinkedList<UUID> m_Children;
};

struct LayoutView
{

	LayoutView(UUID root, Container<UUID, LayoutStructure>&& layouts)
		:	m_RootID(root),
			m_Layouts(layouts)
	{
	}

	UUID m_RootID;
	Container<UUID, LayoutStructure> m_Layouts;


	void merge(UUID attach_id, LayoutView&& layout)
	{
		m_Layouts.merge(std::move(layout.m_Layouts));

		Verify(map_contains(m_Layouts, attach_id));

		LinkedList<UUID> children = std::move(layout.m_Layouts.at(layout.m_RootID).m_Children);

		m_Layouts.at(attach_id).m_Children.emplace_back(std::move(children));
	}
};


enum class LayoutStructureType
{
	ROOT,
	ROW,
	COLUMN,
	ELEMENT
};

class Layout
{
public:


	Layout(Scale&& scale, LayoutStructureType&& layout)
		:	m_ID(Layout::register_child(std::forward<Scale>(scale), std::forward<LayoutStructureType>(layout)))
	{}

	template<typename... Layouts>
	Layout(Scale&& scale, LayoutStructureType&& layout, Layouts&&... layouts)
		:	m_ID(Layout::register_parent(std::forward<Scale>(scale), std::forward<LayoutStructureType>(layout)))
	{}

	operator LayoutView()
	{
		UUID temp = s_CurrentStructure;

		s_CurrentStructure = UUID {};

		return { temp, std::move(s_Layouts)};
	}

	int get_id()
	{
		return m_ID;
	}

private:

	static UUID register_child(Scale&& scale, LayoutStructureType&& layout)
	{
		Verify(map_contains(s_Layouts, s_CurrentStructure));


		UUID id;
		s_Layouts.at(s_CurrentStructure).m_Children.emplace_front(id);
		s_Layouts.insert({ id,{ scale, layout, id, {}  }});

		return id;
	}

	static UUID register_parent(Scale&& scale, LayoutStructureType&& layout)
	{
		LayoutStructure& current_structure = s_Layouts.at(s_CurrentStructure);

		current_structure.m_Scale = scale;
		current_structure.m_LayoutType = layout;

		UUID old_root_id = s_CurrentStructure;

		UUID new_root_id;
		s_Layouts.insert({ new_root_id, { { 100.0, ScaleType::RELATIVE }, LayoutStructureType::ROOT, new_root_id, { old_root_id } }});


		s_CurrentStructure = new_root_id;

		return old_root_id;
	}

	static UUID s_CurrentStructure;

	static Container<UUID, LayoutStructure> s_Layouts;

private:

	int m_ID;
};

UUID Layout::s_CurrentStructure;
Container<UUID, LayoutStructure> Layout::s_Layouts;


class Row : public Layout
{
public:
	template<typename... Tail>
	Row(Scale&& scale, Tail&&... tail)
		: Layout(std::forward<Scale>(scale), LayoutType::ROW, tail...)
	{}
};

class Column : public Layout
{
public:
	template<typename... Tail>
	Column(Scale&& scale, Tail&&... tail)
		: Layout(std::forward<Scale>(scale), LayoutType::COLUMN, tail...)
	{}
};

class Element : public Layout
{
public:
	Element(Scale&& scale)
		: Layout(std::forward<Scale>(scale), LayoutType::CHILD)
	{}
};


Scale operator "" _px(long double scale)
{
	return {
		scale,
		ScaleType::ABSOLUTE
	};
}

Scale operator "" _rel(long double scale)
{
	return {
		scale,
		ScaleType::ABSOLUTE
	};
}

Scale operator "" _max(long double scale)
{
	return {
		scale,
		ScaleType::ABSOLUTE
	};
}

}