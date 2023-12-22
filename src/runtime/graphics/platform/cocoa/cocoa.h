//
// Created by Arjo Nagelhout on 18/12/2023.
//

#ifndef SHAPEREALITY_COCOA_H
#define SHAPEREALITY_COCOA_H

#include <string>
#import <Cocoa/Cocoa.h>

namespace graphics
{
	[[nodiscard]] NSString* toNSString(std::string const& string);

	// converts an NSString to a C-style string
	// warning: this does not return an "owning" pointer. When the NSString is deallocated,
	// the C style string becomes invalid.
	[[nodiscard]] const char* toCString(NSString* string);

	// converts an NSString to a std::string with UTF-8 encoding
	[[nodiscard]] std::string toUtf8String(NSString* string);
}

#endif //SHAPEREALITY_COCOA_H
