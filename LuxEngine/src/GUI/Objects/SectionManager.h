#pragma once

#include "GUIObject.h"

#include "TextManager.h"


namespace Lux::GUI
{

class SectionManager : public GUIObject
{
protected:

	SectionManager()
		: GUIObject(TypeComponent::SECTION)
	{
		auto& rect_component = attach_component<RectComponent>();
		auto& header = attach_component<SectionHeaderComponent>();
		auto& layout = get<LayoutComponent>();


		header.title_text = 0;
		
		layout = section_default_layout();
		attach_component<SectionComponent>();

		std::srand(static_cast<unsigned int>(get_id()));
		float rand_num = ((float)(std::rand() % 100) / 100.0f);

		INFO("Random Number {}", rand_num);
		static const RectComponent DefaultRect
		{
			.tiling = 1.0f,
			.texture = nullptr,
			.color = { 1.0f, 1.0f, 1.0f, 1.0f },
		};


		
		rect_component = DefaultRect;
		
		rect_component.color = { rand_num, 1 - rand_num, 1 - rand_num ,1.0f };
	}

public:

	LayoutComponent section_default_layout() 
	{
		return {
			.scaling_type = ScaleType::DYNAMIC,
			.scale = 1,

			.child_margin = { 10.0f, 10.0f},
			.padding = { 10.0f, 10.0f },

			.spacing = LayoutSpacing::START,
			.parent = 0,
		};
	}

	void make_scalable() 
	{ 
		auto& layout = get<LayoutComponent>();

		layout.scaling_type = ScaleType::DYNAMIC;
		refresh_this();
	}

	void add_decoration()
	{
		auto& section = get<SectionComponent>();
		auto& transform = get<TransformComponent>();
		auto& header = get<SectionHeaderComponent>();

		auto text_manager = new TextManager(section.name, GetFont(), transform.position, { 1.0f, 1.0f, 1.0f, 1.0f});
		
		header.header_height = 20.0f;

		header.title_text = text_manager->get_id();
		section.reserved_top += header.header_height;

		text_manager->fit_to_bbox(transform.scale.x, header.header_height);
	}

	void remove_decoration()
	{
		auto& header = get<SectionHeaderComponent>();
		auto& section = get<SectionComponent>();

		section.reserved_top -= header.header_height;

		GetObject(header.title_text).shutdown();
		header.title_text = 0;
	}


	void set_scale(float rel_dim)
	{
		auto& layout = get<LayoutComponent>();

		float old_scale = layout.scale;
		layout.scale = rel_dim;
		
		if(!layout.parent)
			return refresh_this();
		
		auto& parent_obj = GetObject(layout.parent);
		auto& parent_layout = parent_obj.get<LayoutComponent>();
		if(layout.scaling_type == ScaleType::DYNAMIC)
			parent_layout.sum_relative_scale += rel_dim - old_scale;
		else 
			parent_layout.sum_fixed_scale += rel_dim - old_scale;
		
		parent_obj.refresh_this();
	}

	void attach(GUIObject* child) 
	{ 
		auto& layout		= get<LayoutComponent>();
		auto& name			= child->get<SectionComponent>();
		auto& child_layout	= child->get<LayoutComponent>();

		layout.sections.insert({static_cast<u32>(layout.sections.size()), child->get_id()});

		child->set_parent(get_id());

		if(child_layout.scaling_type == ScaleType::DYNAMIC)
			layout.sum_relative_scale += child_layout.scale;
		else 
			layout.sum_fixed_scale += child_layout.scale;

		refresh_this();
	}

	void detach() 
	{ 
		auto& layout = get<LayoutComponent>();

		UUID parent_id = layout.parent;
		Verify(parent_id);

		GUIObject& parent = GetObject(parent_id);

		auto& parent_layout = parent.get<LayoutComponent>();
		u32 index = 0;

		for(auto& [child_index, child_id] : parent_layout.sections)
		{
			if(get_id() == child_id)
				index = child_index;
		}

		parent_layout.sections.erase(index);

		if(layout.scaling_type == ScaleType::DYNAMIC)
			parent_layout.sum_relative_scale -= layout.scale;
		else 
			parent_layout.sum_fixed_scale -= layout.scale;

		layout.parent = 0;
		
		refresh_this();
	}

	void refresh_header(v2 position, float width, float height, float depth)
	{
		auto& header = get<SectionHeaderComponent>();
		if(!header.title_text)
			return;

		GetObject(header.title_text).refresh(position, width, height, depth);
	}

	void remove_padding()
	{
		auto& style = get<LayoutComponent>();

		style.padding = { 0.0f, 0.0f };

		refresh_this();
	}

};


class HSectionManager final : public SectionManager
{
public:

	HSectionManager(const std::string& title)
	{
		auto& section = get<SectionComponent>();
		section.name = title;
	}

	virtual void refresh(v2 position, float width, float height, float depth) override
	{
		auto& layout = get<LayoutComponent>();
		auto& section = get<SectionComponent>();
		auto& header = get<SectionHeaderComponent>();

		update_transform(position, width, height, depth);

		v2 child_position = { position.x + layout.padding.x, position.y + layout.padding.y};
		float child_width = width - 2 * layout.padding.x;
		float child_height = height - 2 * layout.padding.y - section.reserved_bottom - section.reserved_top;

		refresh_header(child_position, child_width, header.header_height, depth + 1);

		child_position.y += section.reserved_top;

		float relative_size = child_width - layout.sum_fixed_scale;
		
		depth++;
		for(auto& [index, child_id]  : layout.sections)
		{
			auto& child_layout = Get<LayoutComponent>(child_id);

			v2 new_position {  child_position.x + layout.child_margin.x, child_position.y + layout.child_margin.y }; 
			float new_width = 0.0f, new_height = 0.0f;


			float relative_percentage = (float)((float)child_layout.scale / (float)layout.sum_relative_scale);

			if(child_layout.scaling_type == ScaleType::DYNAMIC)
				new_width = relative_size * relative_percentage;
			else 
				new_width = child_layout.scale;
				
			child_position.x += new_width;

			new_height = child_height;
			

			new_width  -= 2 * layout.child_margin.x;
			new_height -= 2 * layout.child_margin.y;

			GetObject(child_id).refresh(new_position, new_width, new_height, depth);
		}
	}
};


class VSectionManager : public SectionManager
{
public:

	VSectionManager(const std::string& title)
	{
		auto& section = get<SectionComponent>();

		section.name = title;
	}

	virtual void refresh(v2 position, float width, float height, float depth) override
	{
		auto& layout = get<LayoutComponent>();
		auto& section = get<SectionComponent>();
		
		auto& header = get<SectionHeaderComponent>();

		
		update_transform(position, width, height, depth);

		v2 child_position = { position.x + layout.padding.x, position.y + layout.padding.y };
		float child_width = width - 2 * layout.padding.x;
		float child_height = height - 2 * layout.padding.y - section.reserved_bottom - section.reserved_top;

		refresh_header(child_position, width, header.header_height, depth + 1);

		child_position.y += section.reserved_top;

		float relative_size = child_height - layout.sum_fixed_scale;
		
		depth++;
		for(auto& [index, child_id]  : layout.sections)
		{
			auto& child_layout = Get<LayoutComponent>(child_id);

			v2 new_position {  child_position.x + layout.child_margin.x, child_position.y + layout.child_margin.y }; 
			float new_width = 0.0f, new_height = 0.0f;


			float relative_percentage = (float)((float)child_layout.scale / (float)layout.sum_relative_scale);

			
			if(child_layout.scaling_type == ScaleType::DYNAMIC)
				new_height = relative_size * relative_percentage;
			else 
				new_height = child_layout.scale;
				
			child_position.y += new_height;
				
			new_width = child_width;

			new_width  -= 2 * layout.child_margin.x;
			new_height -= 2 * layout.child_margin.y;

			GetObject(child_id).refresh(new_position, new_width, new_height, depth);
		}
	}

};

}