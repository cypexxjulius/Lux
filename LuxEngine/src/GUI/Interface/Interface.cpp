#include "Interface.h"

#include "GUI/Objects/SectionObject.h"
#include "GUI/Objects/TextWidgetObject.h"


#include "GUI/Context.h"

#include "GUI/GUILayer.h"

namespace Lux::GUI
{

void HSection::make_scalable() const
{
	m_Manager->make_scalable();
}


void HSection::scale(u32 rel_dim) const
{
	m_Manager->set_scale((float)rel_dim);
}

void HSection::attach(std::initializer_list<GUIObject*> HSections)
{
	for(auto& HSection : HSections)
		m_Manager->attach(HSection);
}

void HSection::remove_padding()
{
	m_Manager->remove_padding();
}

void HSection::attach(GUIObject* HSection)
{
	m_Manager->attach(HSection);
}


void HSection::detach()
{
	m_Manager->detach();
}

void HSection::make_root()
{
	m_Manager->set_root();
}

HSection::HSection(const std::string& title)
	: m_Manager(new HSectionObject(title))
{
}


HSection::operator GUIObject*()
{
	return dynamic_cast<GUIObject*>(m_Manager);
}

void VSection::make_scalable() const
{
	m_Manager->make_scalable();
}


void VSection::scale(u32 rel_dim) const
{
	m_Manager->set_scale((float)rel_dim);
}

void VSection::attach(std::initializer_list<GUIObject*> HSections)
{
	for(auto HSection : HSections)
		m_Manager->attach(HSection);
}

void VSection::remove_padding()
{
	m_Manager->remove_padding();
}

void VSection::attach(GUIObject* HSection)
{
	m_Manager->attach(HSection);
}


void VSection::detach()
{
	m_Manager->detach();
}

void VSection::make_root()
{
	m_Manager->set_root();
}

VSection::VSection(const std::string& title)
	: m_Manager(new VSectionObject(title))
{
}

VSection::operator GUIObject*()
{
	return dynamic_cast<GUIObject*>(m_Manager);
}


void VSection::enable_heading() const
{
	m_Manager->add_decoration();
}

void VSection::disable_heading() const
{
	m_Manager->remove_decoration();
}

void HSection::enable_heading() const
{
	m_Manager->add_decoration();
}

void HSection::disable_heading() const
{
	m_Manager->remove_decoration();
}

TextWidget::TextWidget(const std::string& text)
	: m_Manager(new TextWidgetObject(text, GUIObject::GetFont(), 1.0f))
{
}


void TextWidget::set_text(const std::string& text)
{
	m_Manager->set_text(text);
}

TextWidget::operator GUIObject* ()
{
	return dynamic_cast<GUIObject*>(m_Manager);
}


}

