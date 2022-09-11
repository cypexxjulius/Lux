#pragma once


namespace Lux::GUI
{

class HSectionManager;

class GUIObject;

class HSection
{
private:
	HSection(HSectionManager* manager)
		: m_Manager(manager)
	{
	}

public:

	void make_scalable() const;

	void enable_heading() const;

	void disable_heading() const;

	void scale(u32 rel_dim) const;

	void attach(std::initializer_list<GUIObject*> sections);
	
	void attach(GUIObject* id);

	void remove_padding();

	void detach();

	void make_root();

	operator GUIObject*();

public:

	static HSection Create(const std::string& title);

private:

	HSectionManager* m_Manager = nullptr;

};


class VSectionManager;

class VSection
{
private:
	VSection(VSectionManager* manager)
		: m_Manager(manager)
	{
	}

public:

	void make_scalable() const;
	
	void enable_heading() const;

	void disable_heading() const;

	void scale(u32 rel_dim) const;

	void attach(std::initializer_list<GUIObject*> sections);
	
	void attach(GUIObject* id);

	void remove_padding();

	void detach();

	void make_root();

	operator GUIObject*();

public:

	static VSection Create(const std::string& title);

private:

	VSectionManager* m_Manager = nullptr;

};
}