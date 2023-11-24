cmake_minimum_required(VERSION 3.26)
project(bored_engine)
set(CMAKE_CXX_STANDARD 20)

# ---------- Objective-C support -----------
# https://discourse.cmake.org/t/configure-cmake-to-only-use-objc-linker-only-on-macos-with-pch/3795/4

if (APPLE)
    enable_language(OBJC)
endif ()

# ---------- create target -----------------

set(SOURCES
        # core
        src/main.cpp
        src/types.h
        src/application.h
        src/application.cpp
        src/application_cocoa.mm

        # math
        src/math/rect.h
        src/math/vector.h
        src/math/matrix.h
        src/math/quaternion.h
        src/math/quaternion.cpp
        src/math/vector.cpp

        # renderer
        src/renderer/renderer.h
        src/renderer/renderer.cpp
        src/renderer/window.h
        src/renderer/window_cocoa.h
        src/renderer/window_cocoa.mm
        src/renderer/window.cpp
        src/renderer/pipeline.h
        src/renderer/pipeline.cpp
        src/renderer/shader.h
        src/renderer/shader.cpp
        src/renderer/material.h
        src/renderer/material.cpp
        src/renderer/mesh.h
        src/renderer/mesh.cpp
        src/renderer/texture.h
        src/renderer/texture.cpp
        src/renderer/command_buffer.h
        src/renderer/command_buffer.cpp
        src/renderer/render_pass.h
        src/renderer/render_pass.cpp

        src/renderer/buffer.h
        src/renderer/buffer.cpp

        # renderer backend: metal
        src/renderer/backends/metal/mtl_renderer.h
        src/renderer/backends/metal/mtl_window.h
        src/renderer/backends/metal/mtl_pipeline.h
        src/renderer/backends/metal/mtl_texture.h
        src/renderer/backends/metal/mtl_command_buffer.h

        src/renderer/backends/metal/private/mtl_implementation.h
        src/renderer/backends/metal/private/mtl_renderer.mm
        src/renderer/backends/metal/private/mtl_window.mm
        src/renderer/backends/metal/private/mtl_pipeline.mm
        src/renderer/backends/metal/private/mtl_texture.mm
        src/renderer/backends/metal/private/mtl_command_buffer.mm

        # renderer backend: vulkan
        src/renderer/backends/vulkan/vk_renderer.h
        src/renderer/backends/vulkan/vk_window.h
        src/renderer/backends/vulkan/vk_pipeline.h
        src/renderer/backends/vulkan/vk_texture.h
        src/renderer/backends/vulkan/vk_command_buffer.h

        src/renderer/backends/vulkan/private/vk_renderer.cpp
        src/renderer/backends/vulkan/private/vk_window.cpp
        src/renderer/backends/vulkan/private/vk_pipeline.cpp
        src/renderer/backends/vulkan/private/vk_texture.cpp
        src/renderer/backends/vulkan/private/vk_command_buffer.cpp

        # scene
        src/scene/scene.h
        src/scene/scene.cpp
        src/scene/object.h
        src/scene/object.cpp
        src/scene/layer.h
        src/scene/component.h
        src/scene/component.cpp
        src/scene/transform.h
        src/scene/transform.cpp
)

add_executable(bored_engine ${SOURCES})

# ---------- add subdirectories -------------

add_subdirectory(external)
#add_subdirectory(libs)
#add_subdirectory(src)

set_source_files_properties(src/main.cpp PROPERTIES
        COMPILE_FLAGS "-x objective-c++")

target_link_libraries(bored_engine "-framework Cocoa" "-framework Foundation" "-framework AppKit")

target_link_libraries(bored_engine yaml-cpp metal)