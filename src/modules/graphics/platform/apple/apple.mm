//
// Created by Arjo Nagelhout on 22/01/2024.
//

#include "apple.h"

namespace graphics
{
    NSString* toNSString(std::string const& string)
    {
        return [NSString stringWithCString:string.c_str() encoding:[NSString defaultCStringEncoding]];
    }

    const char* toCString(NSString* string)
    {
        return [string cStringUsingEncoding:[NSString defaultCStringEncoding]];
    }

    std::string toUtf8String(NSString* string)
    {
        NSData* data = [string dataUsingEncoding:NSUTF8StringEncoding];
        std::string stdString(static_cast<char const*>([data bytes]), [data length]);

        return stdString;
    }
}