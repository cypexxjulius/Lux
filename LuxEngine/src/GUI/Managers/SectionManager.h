#pragma once

#include "Manager.h"

#include "GUI/Interface/Interface.h"

#include "GUI/ECS/Components.h"

namespace Lux::GUI
{

#define MAX_FIXED_SCALE (1000)


class SectionManager final : public Manager
{
public:

	UUID create_new()
	{
		UUID id =  create(TypeComponent::SECTION);

		static constexpr SectionStyleComponent DefaultStyle
		{
			.child_margin_x = 1,
			.child_margin_y = 1,
			.padding_x = 1,
			.padding_y = 1,
			.background_color = { 0.8, 0.4, 0.1, 1.0f}
		};

		add_component<SectionStyleComponent>(id) = DefaultStyle;

		static const LayoutComponent DefaultLayout
		{
			.scaling_type = ScaleType::DYNAMIC,
			.scale = 1,

			.spacing = LayoutSpacing::START,
			.orientation = LayoutOrientation::VERTICAL,

			.sum_fixed_scale = 0.0f,
			.sum_relative_scale = 0,
			.parent = 0
		};

		add_component<LayoutComponent>(id) = DefaultLayout;
	
		return id;
	}

	void make_scalable(UUID id) 
	{ 
		auto& layout = get_component<LayoutComponent>(id);

		layout.scaling_type = ScaleType::DYNAMIC;
		layout.scale = 1;
	}

	void recalculate_dimensions(UUID id, iv2 position, float width, float height, float depth)
	{

		auto& layout_component = get_component<LayoutComponent>(id);
		auto& style = get_component<SectionStyleComponent>(id);

		Verify(layout_component.sum_fixed_scale < MAX_FIXED_SCALE);

		position.x += style.padding_x;
		position.y += style.padding_y;

		float relative_size = MAX_FIXED_SCALE - layout_component.sum_fixed_scale;
		for(auto child_id  : layout_component.sections)
		{
			iv2 new_position { 0 };
			float new_width = 0.0, new_height = 0.0;

			auto& child_layout = get_component<LayoutComponent>(id);

			if(layout_component.orientation == LayoutOrientation::HORIZONTAL)
			{
				if(child_layout.scaling_type == ScaleType::DYNAMIC)
					new_width = relative_size * ((float)child_layout.scale / (float)layout_component.sum_relative_scale);
				else 
					new_width = static_cast<float>(child_layout.scale);
				
				new_width -= 2 * style.child_margin_x;
				new_height = height - 2 * style.child_margin_y;
			}
			else 
			{
				if(child_layout.scaling_type == ScaleType::DYNAMIC)
					new_height = relative_size * ((float)child_layout.scale / (float)layout_component.sum_relative_scale);
				else 
					new_height	= static_cast<float>(child_layout.scale);
				
				new_height	-= 2 * style.child_margin_y;
				new_width	= height - 2 * style.child_margin_x;
			}

			new_position = { position.x + style.child_margin_x, position.y + style.child_margin_y };

			recalculate_dimensions(child_id, new_position, new_width, new_height, depth + 1);
		}
	}

	void make_retractable(UUID id) { }

	void set_orientation(UUID id, LayoutOrientation orientation) { }

	void set_scale(UUID id, u32 rel_dim) { }

	void attach(UUID id, UUID child) 
	{ 
		auto& layout = get_component<LayoutComponent>(id);
		auto& child_layout = get_component<LayoutComponent>(child);

		layout.sections.insert(child);

		child_layout.parent = id;

		if(child_layout.scaling_type == ScaleType::DYNAMIC)
			layout.sum_relative_scale += child_layout.scale;
		else 
			layout.sum_fixed_scale += child_layout.scale;
	}

	void detach(UUID id) 
	{ 
		auto& layout = get_component<LayoutComponent>(id);

		UUID parent = layout.parent;
		Verify(parent);

		auto& parent_layout = get_component<LayoutComponent>(id);
		parent_layout.sections.erase(id);

		if(layout.scaling_type == ScaleType::DYNAMIC)
			parent_layout.sum_relative_scale -= layout.scale;
		else 
			parent_layout.sum_fixed_scale -= layout.scale;

		layout.parent = 0;
	}

public:
	
	virtual void on_shutdown() override {}

};

}