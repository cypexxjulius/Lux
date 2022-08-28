#pragma once


namespace Lux::GUI
{

class Context;

class Section
{
public:
	Section(UUID id)
		:	m_ID(id)
	{
	}

	void make_scalable() const;

	void make_retractable() const;

	void scale(u32 rel_dim) const;

	void set_orientation(LayoutOrientation orientation) const;

	void attach(std::initializer_list<Section> sections);
	
	void attach(Section& id);

	void remove_padding();

	void detach();

	void make_root();


public:

	static Section Create(const std::string& title);

private:

	UUID m_ID;

public:
	static void Init(Context& ctx);

	static void Shutdown(Context& ctx);
};
}