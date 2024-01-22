//
// Created by Arjo Nagelhout on 18/12/2023.
//

#ifndef SHAPEREALITY_COCOA_H
#define SHAPEREALITY_COCOA_H

#include <string>
#import <Cocoa/Cocoa.h>

#include "graphics/window.h"

namespace graphics
{
    //
    [[nodiscard]] Rect convert(CGRect value);

    //
    [[nodiscard]] CGRect convert(Rect value);

    //
    [[nodiscard]] Position convert(CGPoint value);

    //
    [[nodiscard]] CGPoint convert(Position value);

    //
    [[nodiscard]] Size convert(CGSize value);

    //
    [[nodiscard]] CGSize convert(Size value);
}

#endif //SHAPEREALITY_COCOA_H
