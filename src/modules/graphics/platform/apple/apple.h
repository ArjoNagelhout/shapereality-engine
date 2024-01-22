//
// Created by Arjo Nagelhout on 22/01/2024.
//

#ifndef SHAPEREALITY_APPLE_H
#define SHAPEREALITY_APPLE_H

#include <string>
#import <Cocoa/Cocoa.h>

namespace graphics
{
    // platform/apple contains utility functions etc. that are common for both cocoa and uikit, such as
    // from the Foundation library

    [[nodiscard]] NSString* toNSString(std::string const& string);

    // converts an NSString to a C-style string
    // warning: this does not return an "owning" pointer. When the NSString is deallocated,
    // the C style string becomes invalid.
    [[nodiscard]] const char* toCString(NSString* string);

    // converts an NSString to a std::string with UTF-8 encoding
    [[nodiscard]] std::string toUtf8String(NSString* string);
}

#endif //SHAPEREALITY_APPLE_H
