#include "LGA.h"


#include "GUILayer.h"

#include "GUI.h"

namespace Lux
{

#define GUI_CHECK() Verify(GUI::GUILayer::Valid())

void LGA::Begin(std::string_view title, v2 position, float width, float height)
{
    GUI_CHECK();
    Verify(GUI::GUILayer::Get().m_active_box == nullptr);

    std::string box_name{ title };

    auto& box_container = GUI::GUILayer::Get().m_boxes;
    if(box_container.find(box_name) != box_container.end())
    {
        box_container.at(box_name).is_valid = true;

        GUI::GUILayer::Get().m_active_box = &box_container.at(box_name);
        return;
    }
    
    box_container.insert({ box_name, { box_name, position, width, height}});
    GUI::GUILayer::Get().m_box_positions.push_back(&box_container.at(box_name));
    GUI::GUILayer::Get().m_active_box = &box_container.at(box_name);
}

void LGA::End()
{
    GUI_CHECK();
    Verify(GUI::GUILayer::Get().m_active_box != nullptr);
    
    GUI::GUILayer::Get().m_active_box = nullptr;
}



}