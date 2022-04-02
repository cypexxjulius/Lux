#pragma once 

#include "GUI.h"
#include "GUILayer.h"

namespace Lux
{



class LGA 
{

    #define GUI_CHECK() using namespace GUI; Verify(GUILayer::Valid())
        
private:


public:

    static void Begin(const std::string& title, v2 position = { 0.0f, 0.0f }, float width = 1.0f, float height = 1.0f)
    {
        GUI_CHECK();
        
        GUILayer::Get().m_boxes.emplace_back(title, position, width, height);
    }

    static void End()
    {
        GUI_CHECK();

        Verify(!GUILayer::Get().m_boxes.empty());
        Verify(!GUILayer::Get().m_boxes.back().is_closed);
    
        GUILayer::Get().m_boxes.back().is_closed = true;
    }
};

}