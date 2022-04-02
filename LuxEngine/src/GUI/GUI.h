#pragma once 

#include "GUILayer.h"


namespace Lux::GUI
{


struct Box
{
    bool is_snapped = false;
    bool is_closed = false;

    float width;
    float height;

    v2 position;
    std::string title;


    Box(const std::string& box_title, v2 box_position, float box_width, float box_height)
        : title(box_title), position(box_position), width(box_width), height(box_height)
    {}
};

}