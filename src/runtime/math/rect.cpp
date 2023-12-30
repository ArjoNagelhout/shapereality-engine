//
// Created by Arjo Nagelhout on 30/11/2023.
//

#include "rect.h"

namespace math
{
    Rect::Rect(float x, float y, float width, float height)
        : x(x), y(y), width(width), height(height)
    {
    }

    Rect::~Rect() = default;
}