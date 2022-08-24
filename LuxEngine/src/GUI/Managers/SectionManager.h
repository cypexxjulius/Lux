#pragma once

#include "GUI/Context.h"

#include "Manager.h"

#include "GUI/Interface/Interface.h"

#include "GUI/ECS/Components.h"

#include "GUI/Space.h"


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
		
		rect_component.color = { rand_num, 1 - rand_num, 1.0f ,1.0f };
		return id;
	}

	void make_scalable(UUID id) 
	{ 
		auto& layout = get_component<LayoutComponent>(id);

		layout.scaling_type = ScaleType::DYNAMIC;
		layout.scale = 1;
	}

	void recalculate_dimensions(UUID id, v2 position, float width, float height, float depth)
	{
		auto& layout_component = get_component<LayoutComponent>(id);
		auto& style = get_component<SectionStyleComponent>(id);

		Verify(layout_component.sum_fixed_scale < MAX_FIXED_SCALE);

		
		update_transform_component(id, position, width, height, depth);

		position.x += style.padding_x;
		position.y += style.padding_y;

		width -= 2 * style.padding_x;
		height -= 2 * style.padding_y;



		float relative_size = MAX_FIXED_SCALE - layout_component.sum_fixed_scale;
		for(auto child_id  : layout_component.sections)
		{
			v2 new_position { 0 };
			float new_width = 0.0, new_height = 0.0;

			auto& child_layout = get_component<LayoutComponent>(id);

			new_position = { position.x + style.child_margin_x, position.y - style.child_margin_y };

			if(layout_component.orientation == LayoutOrientation::HORIZONTAL)
			{
				if(child_layout.scaling_type == ScaleType::DYNAMIC)
				{
					new_width = relative_size * ((float)child_layout.scale / (float)layout_component.sum_relative_scale);
					relative_size -= new_width;
				}
				else 
					new_width = static_cast<float>(child_layout.scale);
				
				position.x += new_width;

				new_width -= 2 * style.child_margin_x;
				new_height = height - 2 * style.child_margin_y;

			}
			else 
			{
				if(child_layout.scaling_type == ScaleType::DYNAMIC)
				{
					new_height = relative_size * ((float)child_layout.scale / (float)layout_component.sum_relative_scale);
					relative_size -= new_height;
				}	
				else 
					new_height	= static_cast<float>(child_layout.scale);
				
				position.y += new_height;
				
				new_height	-= 2 * style.child_margin_y;
				new_width	= width - 2 * style.child_margin_x;
			}
			

			

			depth++;
			recalculate_dimensions(child_id, new_position, new_width, new_height, depth);
		}

	}

	void make_retractable(UUID id) { }

	void set_orientation(UUID id, LayoutOrientation orientation) 
	{ 
		auto& layout = get_component<LayoutComponent>(id);

		layout.orientation = orientation;

		recalculate_section(id);
	}

	void set_scale(UUID id, u32 rel_dim)
	{
		auto& layout = get_component<LayoutComponent>(id);

		layout.scale = rel_dim;
		recalculate_section(id);
	}

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

		auto& transform = get_component<TransformComponent>(id);
		recalculate_section(id);
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
		
		recalculate_section(parent);
	}

	void update_transform_component(UUID id, v2 position, float width, float height, float depth)
	{
		auto& transform = get_component<TransformComponent>(id);

		transform.position = { position.x, position.y, depth};
		transform.scale = { width, height, 1.0f};
	}

	void set_dimensions(float width, float height)
	{
		m_Width = width;
		m_Height = height;
		m_AspectRatio = width / height;
	}
private:

	void recalculate_section(UUID id)
	{
		auto& transform = get_component<TransformComponent>(id);
		recalculate_dimensions(id, { transform.position.x, transform.position.y } , transform.scale.x, transform.scale.y, transform.position.z);
	}

public:
	
	virtual void on_shutdown() override {}

private:

	float m_Width = 0.0f, m_Height = 0.0f, m_AspectRatio = 0.0f;
};

}