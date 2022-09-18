#pragma once

#include "GUIObject.h"

#include "TextManager.h"

#include "RectObject.h"


namespace Lux::GUI
{

static constexpr Array<v4, 4> ColorPallet = {
		v4{ 0.12f, 0.12f, 0.12f, 1.0f},
		v4{ 0.28f, 0.28f, 0.28f, 1.0f},
		v4{ 0.17f, 0.52f, 0.75f, 1.0f},
		v4{0.43f, 0.68f, 0.85f, 1.0f}
};

class SectionManager : public GUIObject
{
public:

	SectionManager()
		: GUIObject(TypeComponent::SECTION)
	{
		auto& section = attach_component<SectionComponent>();
		auto& style = attach_component<SectionStyleComponent>();
		auto& layout = attach_component<LayoutComponent>();

		
		layout = section_default_layout();
		style = section_default_style();

		section.background = CreateObject<RectObject>();
		GetObject<RectObject>(section.background).set_color(ColorPallet[0]);


		section.outline = CreateObject<RectObject>();
		GetObject<RectObject>(section.outline).set_color(ColorPallet[1]);


		section.top_section = CreateObject<RectObject>();
		GetObject<RectObject>(section.top_section).set_color(ColorPallet[2]);
	}

	virtual ~SectionManager()
	{
		auto& section = get<SectionComponent>();
		GetObject(section.background).shutdown();
		GetObject(section.outline).shutdown();
		GetObject(section.top_section).shutdown();

	}

	static LayoutComponent section_default_layout() 
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

	static SectionStyleComponent section_default_style()
	{
		return {
			.top_section_height = 40.0f,
			.bottom_section_height = 0.0f,
			.outline_width = 5.0f,
			.body_padding = 5.0f,

			.top_section_bg_color = ColorPallet[2],
			.bottom_section_bg_color = ColorPallet[2],
			.background_color = ColorPallet[0],
			.outline_color = ColorPallet[1],
			.title_color = ColorPallet[0]
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
		/*
		auto& section = get<SectionComponent>();
		auto& transform = get<TransformComponent>();
		auto& header = get<SectionHeaderComponent>();

		auto text_manager = new TextManager(section.name, GetFont(), transform.position, { 1.0f, 1.0f, 1.0f, 1.0f});
		
		header.header_height = 20.0f;

		header.title_text = text_manager->get_id();
		section.reserved_top += header.header_height;

		text_manager->fit_to_bbox(transform.scale.x, header.header_height);
		*/
	}

	void remove_decoration()
	{
		/*
		auto& header = get<SectionHeaderComponent>();
		auto& section = get<SectionComponent>();

		section.reserved_top -= header.header_height;

		GetObject(header.title_text).shutdown();
		header.title_text = 0;
		*/
	}


	void set_scale(float rel_dim)
	{
		auto& layout = get<LayoutComponent>();

		float old_scale = layout.scale;
		layout.scale = rel_dim;
		
		if(!layout.parent)
			return refresh_this();
		
		auto& parent_obj = GetObject<SectionManager>(layout.parent);
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

	virtual void refresh(v2 position, float width, float height, float depth) override
	{
		auto& section = get<SectionComponent>();
		auto& style = get<SectionStyleComponent>();

		section.position = position;
		section.width = width;
		section.height = height;
		section.depth = depth;

		// Outline Rect
		GetObject<RectObject>(section.outline).refresh(position, width, height, depth++);

		position.x += style.outline_width;
		position.y += style.outline_width;

		width -= style.outline_width * 2;
		height -= style.outline_width * 2;

		// Body Rect
		GetObject<RectObject>(section.background).refresh(position, width, height, depth++);

		position.x += style.body_padding;
		position.y += style.body_padding;

		width -= style.body_padding * 2.0f;
		height -= style.body_padding * 2.0f;

		refresh_body(position, width, height, depth);
	}

	virtual void refresh_body(v2 position, float width, float height, float depth) = 0;

	void refresh_this()
	{
		auto& section = get<SectionComponent>();

		refresh(section.position, section.width, section.height, section.depth);
	}

	void remove_padding()
	{
		auto& style = get<LayoutComponent>();

		style.padding = { 0.0f, 0.0f };

		refresh_this();
	}

};


class HSectionManager : public SectionManager
{
public:

	HSectionManager(const std::string& title)
	{
		auto& section = get<SectionComponent>();
		section.name = title;
	}

	~HSectionManager() = default;
	
	virtual void refresh_body(v2 position, float width, float height, float depth) override
	{
		auto& layout = get<LayoutComponent>();
		auto& section = get<SectionComponent>();

		float relative_size = width - layout.sum_fixed_scale;

		for(auto& [index, child_id]  : layout.sections)
		{
			auto& child_layout = Get<LayoutComponent>(child_id);

			v2 new_position {  position.x + layout.child_margin.x, position.y + layout.child_margin.y }; 
			float new_width = 0.0f, new_height = 0.0f;


			float relative_percentage = (float)((float)child_layout.scale / (float)layout.sum_relative_scale);

			if(child_layout.scaling_type == ScaleType::DYNAMIC)
				new_width = relative_size * relative_percentage;
			else 
				new_width = child_layout.scale;
				
			position.x += new_width;

			new_height = height;
			

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

	
	virtual void refresh_body(v2 position, float width, float height, float depth) override
	{
		auto& layout = get<LayoutComponent>();
		auto& section = get<SectionComponent>();

		float relative_size = height - layout.sum_fixed_scale;
		
		depth++;
		for(auto& [index, child_id]  : layout.sections)
		{
			auto& child_layout = Get<LayoutComponent>(child_id);

			v2 new_position {  position.x + layout.child_margin.x, position.y + layout.child_margin.y }; 
			float new_width = 0.0f, new_height = 0.0f;


			float relative_percentage = (float)((float)child_layout.scale / (float)layout.sum_relative_scale);

			
			if(child_layout.scaling_type == ScaleType::DYNAMIC)
				new_height = relative_size * relative_percentage;
			else 
				new_height = child_layout.scale;
				
			position.y += new_height;
				
			new_width = width;

			new_width  -= 2 * layout.child_margin.x;
			new_height -= 2 * layout.child_margin.y;

			GetObject(child_id).refresh(new_position, new_width, new_height, depth);
		}
	}

};

}