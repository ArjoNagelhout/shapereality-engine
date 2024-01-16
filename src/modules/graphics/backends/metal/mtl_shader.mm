//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_shader.h"

#import <iostream>

#include "mtl_utils.h"
#include "graphics/platform/cocoa/cocoa.h"

namespace graphics
{
    MetalShaderFunction::MetalShaderFunction(id <MTLLibrary> _Nonnull pLibrary,
                                             ShaderFunctionDescriptor const& descriptor)
    {
        MTLFunctionDescriptor* metalDescriptor = [[MTLFunctionDescriptor alloc] init];
        metalDescriptor.name = toNSString(descriptor.entryPoint);

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

    MetalShaderLibrary::MetalShaderLibrary(id <MTLDevice> _Nonnull pDevice, std::filesystem::path const& path)
    {
        // create NSURL from path
        // these types automatically get dereferenced / destroyed when this scope is exited
        NSURL* url = [NSURL fileURLWithPath:toNSString(path)];
        NSError* error = nullptr;
        pLibrary = [pDevice newLibraryWithURL:url error:&error];
        checkMetalError(error, "failed to create MTLLibrary");

        [pLibrary retain];
    }

    MetalShaderLibrary::~MetalShaderLibrary()
    {
        [pLibrary release];
    }

    std::unique_ptr<IShaderFunction>
    MetalShaderLibrary::createShaderFunction(ShaderFunctionDescriptor const& descriptor)
    {
        return std::make_unique<MetalShaderFunction>(pLibrary, descriptor);
    }
}