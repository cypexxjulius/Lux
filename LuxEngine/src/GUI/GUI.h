#pragma once 

#include "Core/Application.h"

namespace Lux::GUI
{



class GUISpace 
{
public:

    static inline v2 ToRenderSpace(const v2& position)
    {
        float aspect_ratio = Application::AspectRatio();
        return { (position.x * 2.0f * aspect_ratio) - aspect_ratio, (position.y * 2.0f - 1.0f) };
    }

    static inline float ToRenderSpaceX(float x)
    {
        float aspect_ratio = Application::AspectRatio();
        return (x * 2.0f * aspect_ratio);
    }

    static inline float ToRenderSpaceY(float y)
    {
        return (y * 2.0f);
    }
    
};


enum class WidgetType
{
    Text,
    Button,
};

struct Widget
{


};

enum class InteractionType
{
    None,
    Selected,
    Holding
};





}