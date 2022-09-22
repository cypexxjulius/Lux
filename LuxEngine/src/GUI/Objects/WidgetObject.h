#pragma once


#include "GUIObject.h"

namespace Lux::GUI
{

class WidgetObject : public GUIObject
{
public:
WidgetObject(TypeComponent type)
	: GUIObject(type)
{
	attach_component<LayoutComponent>();
}



};

}

