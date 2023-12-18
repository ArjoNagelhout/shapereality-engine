//
// Created by Arjo Nagelhout on 18/12/2023.
//

#ifndef BORED_ENGINE_COCOA_H
#define BORED_ENGINE_COCOA_H

#import <Cocoa/Cocoa.h>
#include <string>

namespace common
{
	NSString* toNSString(std::string const& string);

	// warning: this does not return an "owning" pointer. When the NSString is deallocated,
	// the C style string becomes invalid.
	const char* toCString(NSString* string);
}

#endif //BORED_ENGINE_COCOA_H
