#pragma once

#include "GUIChild.h"

#include "GUI/Types.h"

#include "GUI/Layout.h"

namespace Lux::GUI
{

class GUIParent : public GUIChild
{
protected:

	GUIParent(LayoutView&& layout)
		:	GUIChild(1.0f, ScaleType::RELATIVE, GUIType::PARENT), 
			m_Layout(std::forward<LayoutView>(layout))
	{

	}

private:

	LayoutView m_Layout;
};

}