//
// Created by Arjo Nagelhout on 22/01/2024.
//

#include "apple.h"

#include <iostream>

namespace common
{
    NSString* toNSString(std::string const& string)
    {
        return [NSString stringWithCString:string.c_str() encoding:[NSString defaultCStringEncoding]];
    }

    const char* toCString(NSString* string)
    {
        return [string cStringUsingEncoding:[NSString defaultCStringEncoding]];
    }

    std::string toStringUtf8(NSString* string)
    {
        if (string == nullptr)
        {
            return {};
        }

        NSData* data = [string dataUsingEncoding:NSUTF8StringEncoding];
        if (data == nullptr)
        {
            return {};
        }

        std::string stdString(static_cast<char const*>([data bytes]), [data length]);
        return stdString;
    }

    std::filesystem::path toPath(NSURL* url)
    {
        return {url.fileSystemRepresentation};
    }
}