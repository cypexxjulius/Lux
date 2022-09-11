#pragma once

#include "Context.h"

namespace Lux::GUI
{
    class GUISpace
    {
    public:

        static constexpr v3 ToRenderSpaceOrigin(float aspect_ratio)
        {
            return { -aspect_ratio, -1.0f, 0.0f };
        }

        static inline v3 ToRenderSpace(float aspect_ratio, float width, float height, v3 position)
        {
            return { position.x * aspect_ratio / width, position.y / height, position.z / 200 };
        }

        static inline v3 ToRenderSpaceDelta(float aspect_ratio, float width, float height, v3 position)
        {
            return  { position.x * aspect_ratio / width, position.y / height, position.z };
        }

        static inline float ToRenderSpaceX(float aspect_ratio, float x)
        {
            return (x * 2.0f * aspect_ratio);
        }

        static inline float ToRenderSpaceY(float y)
        {
            return (y * 2.0f);
        }

        static inline float FromRenderSpaceX(float aspect_ratio, float x)
        {
            return (x / (2.0f * aspect_ratio));
        }
        static inline float FromRenderSpaceY(float y)
        {
            return (y / 2.0f);
        }

    };

}