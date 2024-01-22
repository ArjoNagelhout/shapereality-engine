//
// Created by Arjo Nagelhout on 18/12/2023.
//

#include "cocoa.h"

namespace graphics
{
    Rect convert(CGRect value)
    {
        return {
            .x = static_cast<float>(value.origin.x),
            .y = static_cast<float>(value.origin.y),
            .width = static_cast<float>(value.size.width),
            .height = static_cast<float>(value.size.height)
        };
    }

    CGRect convert(Rect value)
    {
        return CGRectMake(value.x, value.y, value.width, value.height);
    }

    Position convert(CGPoint value)
    {
        return {
            .x = static_cast<float>(value.x),
            .y = static_cast<float>(value.y)
        };
    }

    CGPoint convert(Position value)
    {
        return CGPointMake(value.x, value.y);
    }

    Size convert(CGSize value)
    {
        return {
            .width = static_cast<float>(value.width),
            .height = static_cast<float>(value.height)
        };
    }

    CGSize convert(Size value)
    {
        return CGSizeMake(value.width, value.height);
    }
}