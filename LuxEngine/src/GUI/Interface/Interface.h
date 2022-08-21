#pragma once 

#include "Core/UUID.h"

namespace Lux::GUI
{

enum class LayoutSpacing
{
	MIDDLE,
	START,
	END,
	MAX_BETWEEN,
	DISTRIBUTED,
};

enum class ScaleType
{
	ZERO_SPACE,
	FIXED,
	DYNAMIC,
};

enum class LayoutOrientation
{
	HORIZONTAL,
	VERTICAL,
};

enum class SectionOption
{
	RETRACTABLE = BIT(0),
	SCALABLE = BIT(1),
	SIZE = 2,
};

enum class WidgetType
{
	HEADER,
	TEXT
};

class InterfaceBase 
{
};

}


#include "SectionInterface.h"