#include "Interface.h"
#include "SectionInterface.h"

#include "GUI/Objects/SectionManager.h"

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

HSection HSection::Create(const std::string& title)
{
	return { new HSectionManager(title) };
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

VSection VSection::Create(const std::string& title)
{
	return { new VSectionManager(title) };
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

}

