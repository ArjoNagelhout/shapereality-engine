//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_shader.h"

#import <cassert>
#import <iostream>

namespace graphics
{
	MetalShaderFunction::MetalShaderFunction(ShaderFunctionDescriptor const& descriptor,
											 id <MTLLibrary> _Nonnull pLibrary)
	{

	}

	MetalShaderFunction::~MetalShaderFunction() = default;

	MetalShaderLibrary::MetalShaderLibrary(std::filesystem::path const& path, id <MTLDevice> _Nonnull pDevice)
	{
		// create NSURL from path
		// these types automatically get dereferenced / destroyed when this scope is exited
		NSString* s = [NSString stringWithCString:path.c_str()
										 encoding:[NSString defaultCStringEncoding]];
		NSURL* url = [NSURL fileURLWithPath:s];
		NSError* libraryError = nil;
		pLibrary = [pDevice newLibraryWithURL:url error:&libraryError];


		if (pLibrary == nullptr)
		{
			std::cout << "failed to create library";

			if (libraryError.description != nullptr)
			{
				// log error
				std::cout << ": "
						  << [libraryError.localizedDescription cStringUsingEncoding:[NSString defaultCStringEncoding]];
			}
			std::cout << std::endl;
			exit(1);
		}

		[pLibrary retain];
	}

	MetalShaderLibrary::~MetalShaderLibrary()
	{
		[pLibrary release];
	}

	std::unique_ptr<IShaderFunction> MetalShaderLibrary::createShaderFunction(ShaderFunctionDescriptor descriptor)
	{
		return std::make_unique<MetalShaderFunction>(descriptor, pLibrary);
	}
}