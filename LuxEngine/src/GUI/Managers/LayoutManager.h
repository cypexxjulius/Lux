#pragma once

#include "Manager.h"

#include "GUI/ECS/Components.h"

namespace Lux::GUI
{

class LayoutManager : public Manager 
{
public:

	static inline LayoutComponent Default(const std::string& name)
	{
		return {
			.name = name,
			.scaling_type = ScaleType::DYNAMIC,
			.scale = 1,

			.spacing = LayoutSpacing::START,
			.orientation = LayoutOrientation::HORIZONTAL,

			.sum_fixed_scale = 0.0f,
			.sum_relative_scale = 0,

			.parent = 0,
		};
	};

	virtual void on_shutdown() override 
	{
	}

	void recalculate_section(UUID id)
	{
		auto& transform = get_component<TransformComponent>(id);
		recalculate_dimensions(id, { transform.position.x, transform.position.y } , transform.scale.x, transform.scale.y, transform.position.z);
	}

	void recalculate_root(UUID id)
	{
		recalculate_dimensions(id, { 0.0f, 0.0f }, m_Width, m_Height, 0);
	}

	void recalculate_dimensions(UUID id, v2 position, float width, float height, float depth)
	{
		auto& layout_component = get_component<LayoutComponent>(id);
		auto& style = get_component<SectionStyleComponent>(id);

		Verify(layout_component.sum_fixed_scale < m_Width);

		
		update_transform_component(id, position, width, height, depth);


		

		v2 child_position = { position.x + style.padding_x, position.y + style.padding_y };
		float child_width = width - 2 * style.padding_x;
		float child_height = height - 2 * style.padding_y;



		float relative_size = -layout_component.sum_fixed_scale;
		if(layout_component.orientation == LayoutOrientation::HORIZONTAL)
			relative_size += child_width;
		else 
			relative_size += child_height;

		depth++;
		for(auto& [index, child_id]  : layout_component.sections)
		{
			auto& child_layout = get_component<LayoutComponent>(child_id);

			v2 new_position {  child_position.x + style.child_margin_x, child_position.y + style.child_margin_y }; 
			float new_width = 0.0f, new_height = 0.0f;


			float relative_percentage = (float)((float)child_layout.scale / (float)layout_component.sum_relative_scale);

			if(layout_component.orientation == LayoutOrientation::HORIZONTAL)
			{	
				if(child_layout.scaling_type == ScaleType::DYNAMIC)
					new_width = relative_size * relative_percentage;
				else 
					new_width = child_layout.scale;
				
				child_position.x += new_width;

				new_height = child_height;
			}
			else 
			{
				if(child_layout.scaling_type == ScaleType::DYNAMIC)
					new_height = relative_size * relative_percentage;
				else 
					new_height = child_layout.scale;
				
				child_position.y += new_height;
				
				new_width = child_width;
			}

			new_width  -= 2 * style.child_margin_x;
			new_height -= 2 * style.child_margin_y;

			recalculate_dimensions(child_id, new_position, new_width, new_height, depth);
		}

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
	}

private:

	float m_Width = 0.0f, m_Height = 0.0f;

};

}