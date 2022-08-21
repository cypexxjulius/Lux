#pragma once


namespace Lux::GUI
{

class Section
{
public:

	Section(UUID id, UUID parent)
		:	m_ID(id)
	{
	}

	void make_scalable() const;

	void make_retractable() const;

	void enable_decoration() const;

	void scale(u32 rel_dim) const;

	void set_orientation(LayoutOrientation orientation) const;

	void attach(std::initializer_list<Section> sections);

	void detach();

public:

	static Section Create(const std::string& title);

private:

	UUID m_ID;

};

void SetMainSection(Section section);

}