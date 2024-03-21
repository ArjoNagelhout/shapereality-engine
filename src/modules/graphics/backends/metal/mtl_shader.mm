//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_shader.h"

#import <iostream>

#include "mtl_utils.h"
#include <common/platform/apple/apple.h>

namespace graphics::metal
{
    MetalShaderFunction::MetalShaderFunction(id <MTLLibrary> _Nonnull library,
                                             ShaderFunctionDescriptor const& descriptor)
    {
        MTLFunctionDescriptor* metalDescriptor = [[MTLFunctionDescriptor alloc] init];
        metalDescriptor.name = common::toNSString(descriptor.entryPoint);

        NSError* error = nullptr;
        function = [library newFunctionWithDescriptor:metalDescriptor error:&error];
        checkMetalError(error, "failed to create MTLFunction");

        [function retain];
    }

    id <MTLFunction> _Nonnull MetalShaderFunction::get()
    {
        return function;
    }

    MetalShaderFunction::~MetalShaderFunction() = default;

    MetalShaderLibrary::MetalShaderLibrary(id <MTLDevice> _Nonnull device, std::filesystem::path const& path)
    {
        // create NSURL from path
        // these types automatically get dereferenced / destroyed when this scope is exited
        NSURL* url = [NSURL fileURLWithPath:common::toNSString(path)];
        NSError* error = nullptr;
        library = [device newLibraryWithURL:url error:&error];
        checkMetalError(error, "failed to create MTLLibrary");

        [library retain];
    }

    MetalShaderLibrary::~MetalShaderLibrary()
    {
        [library release];
    }

    std::unique_ptr<IShaderFunction>
    MetalShaderLibrary::createShaderFunction(ShaderFunctionDescriptor const& descriptor)
    {
        return std::make_unique<MetalShaderFunction>(library, descriptor);
    }
}