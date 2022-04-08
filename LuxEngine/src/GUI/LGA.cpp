#include "LGA.h"


#include "GUILayer.h"

#include "GUI.h"

namespace Lux
{

#define GUI_CHECK() Verify(GUI::GUILayer::Valid())

void LGA::Begin(std::string_view title, v2 position, float width, float height)
{
    GUI_CHECK();

    std::string box_name{ title };

    auto& box_container = GUI::GUILayer::Get().m_boxes;
    if(box_container.find(box_name) != box_container.end())
    {
        box_container.at(box_name).is_valid = true;
        return;
    }
    
    box_container.insert({ box_name, { box_name, position, width, height}});
    GUI::GUILayer::Get().m_box_positions.push_back(&box_container.at(box_name));
}

void LGA::End()
{
    GUI_CHECK();

    Verify(!GUI::GUILayer::Get().m_box_positions.empty());
    Verify(!GUI::GUILayer::Get().m_box_positions.back()->is_closed);
    
    GUI::GUILayer::Get().m_box_positions.back()->is_closed = true;
}



}