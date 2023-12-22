//
// Created by Arjo Nagelhout on 14/12/2023.
//

#ifndef BORED_ENGINE_MTL_UTILS_H
#define BORED_ENGINE_MTL_UTILS_H

#include <string>

#import <Metal/Metal.h>

namespace graphics
{
	void checkMetalError(NSError* error, std::string const& messageIfFailed);
}

#endif //BORED_ENGINE_MTL_UTILS_H
