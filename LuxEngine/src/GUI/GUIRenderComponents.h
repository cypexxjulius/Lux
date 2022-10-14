#pragma once

#include "Utils/Types.h"

#include "Assets/Texture.h"

#include "Assets/Font.h"


namespace Lux::GUI
{

struct TransformComponent
{
	v3 position;
	v3 scale;
	v3 rotation;
};

struct Rect
{
	v4 color;
	Ref<Texture> texture;
	float tiling;
};

struct Glyph
{
	char glyph;
	v4 color;
	Ref<Font> font;
	Array<v2, 4> tex_coords;
	mat4 char_transform;
};



}