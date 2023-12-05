project(bored_engine_library)

# ---------- create target -----------------

set(SOURCES_LIBRARY
        # core
        types.h
        application.h
        application.cpp
        application_cocoa.mm

        # math
        math/rect.h
        math/vector.h
        math/vector.inl
        math/matrix.h
        math/quaternion.h
        math/quaternion.inl
        math/plane.h
        math/bounds.h
        math/bounds.inl

        math/ray.h
        math/ray.inl
        math/ray.cpp
        math/rect.cpp
        math/matrix.inl
        math/plane.inl

        # renderer
        renderer/graphics.h
        renderer/graphics.cpp
        renderer/window.h
        renderer/window_cocoa.h
        renderer/window_cocoa.mm
        renderer/window.cpp
        renderer/pipeline.h
        renderer/pipeline.cpp
        renderer/shader.h
        renderer/shader.cpp
        renderer/material.h
        renderer/material.cpp
        renderer/mesh.h
        renderer/mesh.cpp
        renderer/texture.h
        renderer/texture.cpp
        renderer/command_buffer.h
        renderer/command_buffer.cpp
        renderer/render_pass.h
        renderer/render_pass.cpp

        renderer/buffer.h
        renderer/buffer.cpp

        # renderer backend: metal
        renderer/backends/metal/mtl_graphics.h
        renderer/backends/metal/mtl_window.h
        renderer/backends/metal/mtl_pipeline.h
        renderer/backends/metal/mtl_texture.h
        renderer/backends/metal/mtl_command_buffer.h

        renderer/backends/metal/private/mtl_implementation.h
        renderer/backends/metal/private/mtl_graphics.mm
        renderer/backends/metal/private/mtl_window.mm
        renderer/backends/metal/private/mtl_pipeline.mm
        renderer/backends/metal/private/mtl_texture.mm
        renderer/backends/metal/private/mtl_command_buffer.mm

        # renderer backend: vulkan
        renderer/backends/vulkan/vk_graphics.h
        renderer/backends/vulkan/vk_window.h
        renderer/backends/vulkan/vk_pipeline.h
        renderer/backends/vulkan/vk_texture.h
        renderer/backends/vulkan/vk_command_buffer.h

        renderer/backends/vulkan/private/vk_renderer.cpp
        renderer/backends/vulkan/private/vk_window.cpp
        renderer/backends/vulkan/private/vk_pipeline.cpp
        renderer/backends/vulkan/private/vk_texture.cpp
        renderer/backends/vulkan/private/vk_command_buffer.cpp

        # scene
        scene/scene.h
        scene/scene.cpp
        scene/object.h
        scene/object.cpp
        scene/layer.h
        scene/component.h
        scene/component.cpp
        scene/transform.h
        scene/transform.cpp
)

add_library(bored_engine_library ${SOURCES_LIBRARY})
target_include_directories(bored_engine_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bored_engine_library
        "-framework Cocoa"
        "-framework Foundation"
        "-framework AppKit"
        "-framework Metal"
        "-framework MetalKit"
)

set(SOURCES
        main.cpp
)

add_executable(bored_engine ${SOURCES})
target_link_libraries(bored_engine bored_engine_library)