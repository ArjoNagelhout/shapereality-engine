//
// Created by Arjo Nagelhout on 18/12/2023.
//

#include "cocoa.h"

namespace graphics
{
	NSString* toNSString(std::string const& string)
	{
		return [NSString stringWithCString:string.c_str()];
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