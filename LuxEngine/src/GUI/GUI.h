#pragma once 

#include "Core/Application.h"

namespace Lux::GUI
{



class GUISpace 
{
public:

    static constexpr v2 ToRenderSpaceOrigin()
    {
        return { -Application::AspectRatio(), -1.0f };
    }

    static inline v2 ToRenderSpace(const v2& position)
    {
        return { (position.x * 2.0f * Application::AspectRatio()) - Application::AspectRatio(), (position.y * 2.0f - 1.0f) };
    }

    static inline v2 ToRenderSpaceDelta(v2 position)
    {
        return { position.x * 2.0f * Application::AspectRatio(), position.y * 2.0f };
    }

    static inline float ToRenderSpaceX(float x)
    {
        return (x * 2.0f * Application::AspectRatio());
    }

    static inline float ToRenderSpaceY(float y)
    {
        return (y * 2.0f);
    }

    static inline float FromRenderSpaceX(float x)
    {
        return (x / (2.0f * Application::AspectRatio()));
    }
    static inline float FromRenderSpaceY(float y)
    {
        return (y / 2.0f);
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