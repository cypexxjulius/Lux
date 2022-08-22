#pragma once

#include "Manager.h"

#include "GUI/Interface/Interface.h"

#include "GUI/ECS/Components.h"

namespace Lux::GUI
{


class SectionManager final : public Manager
{
public:

	UUID create_new()
	{
		return create();
	}

	void make_scalable(UUID id) 
	{ 
		auto& layout_component = get_component<LayoutComponent>(id);

		layout_component.scale = 1.0f;
	}

	void make_retractable(UUID id) { }

	void set_orientation(UUID id, LayoutOrientation orientation) { }

	void set_scale(UUID id, u32 rel_dim) { }

	void attach(UUID id, UUID child) { }

	void detach(UUID id) { }

public:
	
	virtual void on_shutdown() override {}

};

}