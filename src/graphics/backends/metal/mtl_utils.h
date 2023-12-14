//
// Created by Arjo Nagelhout on 14/12/2023.
//

#ifndef BORED_ENGINE_MTL_UTILS_H
#define BORED_ENGINE_MTL_UTILS_H

#include <string>

#import <Metal/Metal.h>

namespace graphics
{
	NSString* toNSString(std::string const& string);

	// warning: this does not return an "owning" pointer. When the NSString is deallocated,
	// the C style string becomes invalid.
	const char* toCString(NSString* string);

	void checkMetalError(NSError* error, std::string const& messageIfFailed);
}

#endif //BORED_ENGINE_MTL_UTILS_H
