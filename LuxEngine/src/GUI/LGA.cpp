#include "LGA.h"


#include "GUILayer.h"

#include "GUI.h"

namespace Lux
{

#define GUI_CHECK() Verify(GUI::GUILayer::Valid())

void LGA::Begin(const std::string& title, v2 position, float width, float height)
{
    GUI_CHECK();
        
    GUI::GUILayer::Get().m_boxes.emplace_back(title, position, width, height);
}

void LGA::End()
{
    GUI_CHECK();

    Verify(!GUI::GUILayer::Get().m_boxes.empty());
    Verify(!GUI::GUILayer::Get().m_boxes.back().is_closed);
    
    GUI::GUILayer::Get().m_boxes.back().is_closed = true;
}



}