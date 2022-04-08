#pragma once 

#include "Core/Application.h"

#include "GUILayer.h"

#include "Graphics/Renderer2D.h"
#include "GUIStyle.h"

#include <map>

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

struct Box
{

    bool is_valid = true;
    bool is_snapped = false;
    bool is_closed = false;
    bool is_dragged = false;
    bool is_selected = false;

    float width, height;
    float min_width, min_height;

    v2 position;
    std::string title;

    // std::map<std::string, Widget> widgets;

    Renderable2D<Renderable2DType::Rect> render_component;
    Renderable2D<Renderable2DType::Text> text_render_component;

    Box(const std::string& box_title, v2 box_position, float box_width, float box_height)
        :   title(box_title), 
            position(box_position), 
            width(box_width), 
            height(box_height), 
            render_component(GUISpace::ToRenderSpace(box_position), GUISpace::ToRenderSpaceX(box_width), GUISpace::ToRenderSpaceY(box_height), Style::box_background_color),
            text_render_component(box_title, GUISpace::ToRenderSpace(box_position), 1.0f)
    {}

    bool inline is_intersecting(v2 check_position) const 
    {
        return  check_position.x > position.x &&
                check_position.x < position.x + width &&
                check_position.y > position.y &&
                check_position.y < position.y + height;
    }

    void inline add_position_vector(v2 delta)
    {
        position += delta;
        render_component.reset_transform(GUISpace::ToRenderSpace(position), GUISpace::ToRenderSpaceX(width), GUISpace::ToRenderSpaceY(height));
        text_render_component.reset_transform(title, GUISpace::ToRenderSpace(position), 1.0f);
    }

    void inline select() 
    {
        is_dragged = true;
        render_component.set_color(Style::selected_box_background_color);
    }

    void inline unselect()
    {
        is_dragged = is_selected = false;
        render_component.set_color(Style::box_background_color);
    }

    void inline drag()
    {
        is_dragged = is_selected = true;
        render_component.set_color(Style::selected_box_background_color);
    }

    void inline undrag()
    {
        is_dragged = false;
    }

};



}