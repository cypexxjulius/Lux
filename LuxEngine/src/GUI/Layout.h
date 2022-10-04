#pragma once

#include "Utils/Types.h"

#include "Types.h"

namespace Lux::GUI
{

class Layout
{
public:

	Layout( )

private:

	Scale m_Scale;
	LayoutType m_Layout;

	LinkedList<Layout> m_Children;
};

}