#include "Types.h"

/*

struct Box
{
    static constexpr float ResizeBoxMargin = 0.005f;

    bool is_valid = true;
    bool is_snapped = false;
    InteractionType interaction = InteractionType::None;

    float width, height;
    float min_width, min_height;

    v2 position;
    std::string title;

    ResizingPoint rpoint;

    // std::map<std::string, Widget> widgets;

    Renderable2D<Renderable2DType::Rect> render_component;
    Renderable2D<Renderable2DType::Text> text_render_component;

    Box(const std::string& box_title, v2 box_position, float box_width, float box_height)
        :   title(box_title), 
            position(box_position), 
            width(box_width), 
            height(box_height), 
            render_component(GUISpace::ToRenderSpace(box_position), GUISpace::ToRenderSpaceX(box_width + GUI::Style::title_text_padding * 2), GUISpace::ToRenderSpaceY(box_height + GUI::Style::title_text_padding * 2), Style::box_background_color),
            text_render_component(box_title, GUISpace::ToRenderSpaceOrigin(), 0.3f)
    {
        const auto[ minimal_width, minimal_height ] = Renderer2D::Dimensions(text_render_component);
        min_width = GUISpace::FromRenderSpaceX(minimal_width) + GUI::Style::title_text_padding * 2;
        min_height = GUISpace::FromRenderSpaceY(minimal_height) + GUI::Style::title_text_padding * 2;
    }

    bool inline is_intersecting(v2 check_position) const 
    {
        return  check_position.x > position.x - ResizeBoxMargin&&
                check_position.x < position.x + width + ResizeBoxMargin &&
                check_position.y > position.y - ResizeBoxMargin&&
                check_position.y < position.y + height + ResizeBoxMargin;
    }

    void inline check_resizing_point(v2 check_position)
    {

        if( check_position.x > position.x - ResizeBoxMargin &&
            check_position.x < position.x + ResizeBoxMargin &&
            check_position.y > position.y &&
            check_position.y < position.y + height
        ) rpoint = ResizingPoint::Left;

        else if(check_position.x > position.x + width - ResizeBoxMargin &&
            check_position.x < position.x + width + ResizeBoxMargin &&
            check_position.y > position.y &&
            check_position.y < position.y + height
        ) rpoint = ResizingPoint::Right;



        if( check_position.x > position.x &&
            check_position.x < position.x + width &&
            check_position.y > position.y - ResizeBoxMargin &&
            check_position.y < position.y + ResizeBoxMargin
        )
        {
            if(rpoint == ResizingPoint::Left)
                rpoint = ResizingPoint::TopLeft;
            else if(rpoint == ResizingPoint::Right)
                rpoint = ResizingPoint::TopRight;
            else 
                rpoint = ResizingPoint::Top;

            return;
        }

        if(    check_position.x > position.x &&
                    check_position.x < position.x + width &&
                    check_position.y > position.y + height - ResizeBoxMargin &&
                    check_position.y < position.y + height + ResizeBoxMargin
        )
        {
            if(rpoint == ResizingPoint::Left)
                rpoint = ResizingPoint::BottomLeft;
            else if(rpoint == ResizingPoint::Right)
                rpoint = ResizingPoint::BottomRight;
            else 
                rpoint = ResizingPoint::Bottom;

            return;
        }

    }

    inline void update_render_components()
    {
        render_component.reset_transform(GUISpace::ToRenderSpace(position), GUISpace::ToRenderSpaceX(width), GUISpace::ToRenderSpaceY(height));
    }

    inline void on_select(v2 new_position)
    {
        render_component.set_color(Style::selected_box_background_color);
        check_resizing_point(new_position);
    }

    inline void on_hold()
    {

    }

    inline void on_unhold()
    {
        if(rpoint != ResizingPoint::None)
            rpoint = ResizingPoint::None;
    }

    inline void on_mousedelta(v2 delta)
    {
        switch(rpoint)
        {
            case ResizingPoint::None:
                position += delta;
                break;

            case ResizingPoint::Left:
                position.x += delta.x;
                width -= delta.x;
                break;

            case ResizingPoint::Right:
                width += delta.x;
                break;

            case ResizingPoint::Top:
                position.y += delta.y;
                height -= delta.y;
                break;

            case ResizingPoint::Bottom:
                height += delta.y;
                break;
            
            
            case ResizingPoint::TopLeft:
                position.x += delta.x;
                width -= delta.x;
                position.y += delta.y;
                height -= delta.y;
                break;
  
            case ResizingPoint::TopRight:
                width += delta.x;
                position.y += delta.y;
                height -= delta.y;
                break;

            case ResizingPoint::BottomLeft:
                position.x += delta.x;
                width -= delta.x;
                height += delta.y;
                break;

            case ResizingPoint::BottomRight:
                width += delta.x;
                height += delta.y;
                break;

        }
        width = std::max(width, min_width);
        height = std::max(height, min_height);

        update_render_components();
    }

    inline void on_unselect()
    {
        render_component.set_color(Style::box_background_color);
    }

    
    inline CursorType on_hover(v2 check_position)
    {
        check_resizing_point(check_position);

        auto temp_rpoint = rpoint;
        rpoint = ResizingPoint::None;

        switch (temp_rpoint)
        {
            case ResizingPoint::Top:
            case ResizingPoint::Bottom:
                return CursorType::VResize;
            
            case ResizingPoint::Left:
            case ResizingPoint::Right:
                return CursorType::HResize;

            case ResizingPoint::TopLeft:
            case ResizingPoint::BottomRight:
                return CursorType::HVResizeTB;

            case ResizingPoint::TopRight:
            case ResizingPoint::BottomLeft:
                return CursorType::HVResizeBT;

            case ResizingPoint::None:
                return CursorType::PointingHand;
        }

        return CursorType::Hand;
    }

    inline void draw()
    {

        Renderer2D::Draw(render_component);

        Renderer2D::PushOrigin(GUISpace::ToRenderSpaceDelta(position));

            Renderer2D::PushOrigin(GUISpace::ToRenderSpaceDelta({ GUI::Style::title_text_padding, GUI::Style::title_text_padding}));

            Renderer2D::Draw(text_render_component);

            Renderer2D::PopTransform();

        Renderer2D::PopTransform();
    }

};

*/