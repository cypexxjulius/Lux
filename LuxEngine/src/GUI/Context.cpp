#include "Context.h"

#include "ECS/Components.h"

#include "Managers/SectionManager.h"
#include "Interface/SectionInterface.h"

namespace Lux::GUI
{


Context::Context()
{
	m_Registry.register_component<TransformComponent, LayoutComponent, LayoutInfo, RectComponent, TextComponent, GlyphComponent>();

	connect<Section, SectionManager>();
}

}