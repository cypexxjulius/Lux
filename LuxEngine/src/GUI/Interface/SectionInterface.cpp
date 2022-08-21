#include "Interface.h"
#include "SectionInterface.h"

#include "GUI/Managers/SectionManager.h"

#include "GUI/Context.h"

#include "GUI/GUILayer.h"

namespace Lux::GUI
{

SectionManager* s_Manager = nullptr;



void Section::Init(Context& ctx)
{

	s_Manager = &ctx.get_manager<SectionManager>();
}

void Section::Shutdown(Context& ctx)
{
}

void Section::make_scalable() const
{
	s_Manager->make_scalable(m_ID);
}

void Section::make_retractable() const
{
	s_Manager->make_retractable(m_ID);
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

void Section::make_root()
{
	GUILayer::set_root(m_ID);
}

Section Section::Create(const std::string& title)
{
	return Section{ s_Manager->create() };
}

}

