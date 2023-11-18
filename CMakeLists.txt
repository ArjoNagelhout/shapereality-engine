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
        src/main.cpp
        src/types.h
        src/application.h
        src/application.cpp
        src/application_cocoa.mm
        src/window.h
        src/window_cocoa.mm
        src/view.h
        src/view_cocoa.h
        src/view_cocoa.mm

        src/renderer/renderer.h
        src/renderer/renderer.cpp
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
        src/renderer/pipeline.h
        src/renderer/pipeline.cpp

        src/renderer/backends/metal/mtl_renderer.h
        src/renderer/backends/metal/mtl_renderer.cpp
        src/renderer/backends/vulkan/vk_renderer.h
        src/renderer/backends/vulkan/vk_renderer.cpp
        src/renderer/backends/metal/mtl_view.h
        src/renderer/backends/metal/mtl_view.mm
        src/renderer/backends/metal/mtl_texture.h
        src/renderer/backends/metal/mtl_texture.cpp

        src/scene/scene.h
        src/scene/scene.cpp
        src/scene/object.h
        src/scene/object.cpp
        src/scene/layer.h
        src/scene/component.h
        src/scene/component.cpp
        src/scene/transform.h
        src/scene/transform.cpp

        src/math/rect.h
        src/math/vector.h
        src/math/matrix.h
        src/math/quaternion.h
        src/math/quaternion.cpp
        src/math/vector.cpp
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