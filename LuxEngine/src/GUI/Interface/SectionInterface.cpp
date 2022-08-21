#include "Interface.h"
#include "SectionInterface.h"

#include "GUI/Managers/SectionManager.h"

namespace Lux::GUI
{

SectionManager* s_Manager = nullptr;

void SectionSetManager(SectionManager* manager)
{
	Verify(manager);

	s_Manager = manager;
}

void Section::make_scalable() const
{
	s_Manager->make_scalable(m_ID);
}

void Section::make_retractable() const
{
	s_Manager->make_retractable(m_ID);
}

void Section::enable_decoration() const 
{
	s_Manager->enable_decoration(m_ID);
}

void Section::set_orientation(LayoutOrientation orientation) const
{
	s_Manager->set_orientation(m_ID, orientation);
}

void Section::scale(u32 rel_dim) const
{
	s_Manager->set_scale(m_ID, rel_dim);
}

void Section::attach(std::initializer_list<Section> sections)
{
	for(auto& section : sections)
		s_Manager->attach(m_ID, section.m_ID);
}

void Section::detach()
{
	s_Manager->detach(m_ID);
}

Section CreateSection(const std::string& title)
{
	return GUILayer::CreateSection(title);
}

Section Section::Create(const std::string& title)
{
	return s_Manager.create(title);
}

void Section::SetManager(Manager* manager)
{
	s_Manager = dynamic_cast<SectionManager*>(manager);
}

}

