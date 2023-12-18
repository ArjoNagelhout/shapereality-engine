//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_shader.h"

#import <iostream>

#include "mtl_utils.h"
#import "common/platform/cocoa/cocoa.h"

namespace graphics
{
	MetalShaderFunction::MetalShaderFunction(ShaderFunctionDescriptor const& descriptor,
											 id <MTLLibrary> _Nonnull pLibrary)
	{
		MTLFunctionDescriptor* metalDescriptor = [[MTLFunctionDescriptor alloc] init];
		metalDescriptor.name = common::toNSString(descriptor.entryPoint);

		NSError* error = nullptr;
		pFunction = [pLibrary newFunctionWithDescriptor:metalDescriptor error:&error];
		checkMetalError(error, "failed to create MTLFunction");

		[pFunction retain];
	}

	id <MTLFunction> _Nonnull MetalShaderFunction::getFunction()
	{
		return pFunction;
	}

	MetalShaderFunction::~MetalShaderFunction() = default;

	MetalShaderLibrary::MetalShaderLibrary(std::filesystem::path const& path, id <MTLDevice> _Nonnull pDevice)
	{
		// create NSURL from path
		// these types automatically get dereferenced / destroyed when this scope is exited
		NSURL* url = [NSURL fileURLWithPath:common::toNSString(path)];
		NSError* error = nullptr;
		pLibrary = [pDevice newLibraryWithURL:url error:&error];
		checkMetalError(error, "failed to create MTLLibrary");

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