#pragma once


#include "Core/UUID.h"

#include "GUI/Types.h"

namespace Lux::GUI
{

class GUIObject;

class HSectionObject;

class VSectionObject;

class TextWidgetObject;

class HSection
{
public:
	HSection(const std::string& title);

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

private:

	HSectionObject* m_Manager = nullptr;

};

class VSection
{
public:

	VSection(const std::string& title);

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


private:

	VSectionObject* m_Manager;

};

class TextWidget
{
public:

	TextWidget(const std::string& title);

	void set_text(const std::string& text);


	operator GUIObject* ();

private:
	TextWidgetObject* m_Manager;
};

}