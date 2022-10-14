#pragma once

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
	RELATIVE,
	MIN,
};

enum class LayoutOrientation
{
	HORIZONTAL,
	VERTICAL,
};

enum class LayoutType
{
	CHILD,
	ROW,
	COLUMN
};

struct Scale
{
	long double scale;
	ScaleType type;
};

Scale operator "" _px(long double scale)
{
	return {
		scale,
		ScaleType::FIXED
	};
}

Scale operator "" _rel(long double scale)
{
	return {
		scale,
		ScaleType::RELATIVE
	};
}


}
