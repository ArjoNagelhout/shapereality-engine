//
// Created by Arjo Nagelhout on 11/04/2024.
//

#ifndef SHAPEREALITY_SHADER_SHADER_H
#define SHAPEREALITY_SHADER_SHADER_H

namespace shader
{
    // todo: shader compilation
    // late night notes:
    // Should we write a shader compiler? What a lovely, terrible idea haha.
    // There goes 3 months again minimum. Too much surface area for the code base.
    // Maybe leverage LLVM or equivalent? Don't want to be doing the parsing manually.
    // Parser generator? Shader compilation is something that might be bypassed by doing some preprocessing.
    // However, I suspect that this will quickly grow too complex.

    // But it is something that should be done by the engine. Maybe a separate process.
    // However, this is quite complex. Cross-compiling might also be worthwhile?
    // In that case we could compile metal shaders on a non-Apple platform.

    // We don't need to compile the shader itself. We can generate the .metal.
    // Or we use a SPIR-V to Intermediate Representation compiler / transpiler.
    //
    // requirements:
    // - we want to generate shaders
    //
    // https://gameworldobserver.com/2023/04/07/shader-compilation-stuttering-pc-games-explained
    // https://www.reddit.com/r/nvidia/comments/8xufkn/why_dont_games_just_compile_all_shaders_during/
    //
    // apparently this has become a user-facing issue
    //
    // Metal Shading Language (MSL) (.metal file extension, plain text)
    // gets compiled into
    // Intermediate Representation (IR) (.air file extension,
    // by the Metal compiler (based on Clang / LLVM)
    //
    // Steps: preprocess, compile, assemble source file
    //
    // using metallib the IR files can be put into a single Metal library.
    // each shader inside the library should have a unique name
    // there is no separation between the different IR files.
    // so you can't have shader_1.metal and shader_2.metal that each contain [[vertex]] void main(...) {...}

    //
    // https://developer.apple.com/documentation/metal/shader_libraries/building_a_shader_library_by_precompiling_source_files?language=objc
    // metal shaders can be compiled at runtime, even from their .metal files
    //
    // we need to compile shaders to both Metal IR and SPIR-V. Is there some library that already does that?
    // and what would the input data be? .glsl + a meta language similar to Unity's ShaderLab? https://docs.unity3d.com/Manual/SL-Reference.html
    // e.g. which properties to expose to the Material inspector. 
}

#endif //SHAPEREALITY_SHADER_SHADER_H
