//
// Created by Arjo Nagelhout on 18/12/2023.
//

#include "cocoa.h"

namespace common
{
	NSString* toNSString(std::string const& string)
	{
		return [NSString stringWithCString:string.c_str()];
	}

	const char* toCString(NSString* string)
	{
		return [string cStringUsingEncoding:[NSString defaultCStringEncoding]];
	}
}