#pragma once

#include "GUI/Context.h"

#include "Manager.h"

#include "GUI/Interface/Interface.h"

#include "GUI/ECS/Components.h"


namespace Lux::GUI
{

class SectionManager final : public Manager
{
public:

	UUID create_new(const std::string& name)
	{
		UUID id =  create(TypeComponent::SECTION, name);

		static constexpr SectionStyleComponent DefaultStyle
		{
			.child_margin_x = 10,
			.child_margin_y = 10,
			.padding_x = 10,
			.padding_y = 10,
			.background_color = { 0.8, 0.4, 0.1, 1.0f}
		};

		add_component<SectionStyleComponent>(id) = DefaultStyle;
	
		std::srand(static_cast<unsigned int>(id));
		float rand_num = ((float)(std::rand() % 100) / 100.0f);

		INFO("Random Number {}", rand_num);
		static const RectComponent DefaultRect
		{
			.tiling = 1.0f,
			.texture = nullptr,
			.color = { 1.0f, 1.0f, 1.0f, 1.0f }, //DefaultStyle.background_color,
		};

		auto& rect_component = add_component<RectComponent>(id);
		
		rect_component = DefaultRect;
		
		rect_component.color = { rand_num, 1 - rand_num, 1 - rand_num ,1.0f };
		return id;
	}

	void make_scalable(UUID id) 
	{ 
		auto& layout = get_component<LayoutComponent>(id);

		layout.scaling_type = ScaleType::DYNAMIC;
		layout.scale = 1;

		refresh_section(id);
	}

	void make_retractable(UUID id) { }

	void set_orientation(UUID id, LayoutOrientation orientation) 
	{ 
		auto& layout = get_component<LayoutComponent>(id);

		layout.orientation = orientation;

		refresh_section(id);
	}

	void set_scale(UUID id, u32 rel_dim)
	{
		auto& layout = get_component<LayoutComponent>(id);

		layout.scale = (float)rel_dim;
		refresh_section(id);
	}

	void attach(UUID id, UUID child) 
	{ 
		auto& layout = get_component<LayoutComponent>(id);
		auto& child_layout = get_component<LayoutComponent>(child);

		layout.sections.insert({static_cast<u32>(layout.sections.size()), child});

		child_layout.parent = id;

		if(child_layout.scaling_type == ScaleType::DYNAMIC)
			layout.sum_relative_scale += child_layout.scale;
		else 
			layout.sum_fixed_scale += child_layout.scale;

		refresh_section(id);
	}

	void detach(UUID id) 
	{ 
		auto& layout = get_component<LayoutComponent>(id);

		UUID parent = layout.parent;
		Verify(parent);

		auto& parent_layout = get_component<LayoutComponent>(id);
		u32 index = 0;

		for(auto& [child_index, child_id] : parent_layout.sections)
		{
			if(id == child_id)
				index = child_index;
		}
		parent_layout.sections.erase({ index, id });

		if(layout.scaling_type == ScaleType::DYNAMIC)
			parent_layout.sum_relative_scale -= layout.scale;
		else 
			parent_layout.sum_fixed_scale -= layout.scale;

		layout.parent = 0;
		
		refresh_section(parent);
	}

	void remove_padding(UUID id)
	{
		auto& style = get_component<SectionStyleComponent>(id);

		style.padding_x = style.padding_y = 0;

		refresh_section(id);
	}

public:
	
	virtual void on_shutdown() override {}

};

}