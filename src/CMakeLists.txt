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
        math/matrix.cpp
        math/quaternion.h
        math/quaternion.cpp
        math/plane.h
        math/bounds.h
        math/bounds.inl
        math/bounds.cpp
        math/ray.h
        math/ray.inl
        math/ray.cpp
        math/plane.cpp
        math/rect.cpp

        # renderer
        renderer/renderer.h
        renderer/renderer.cpp
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
        renderer/backends/metal/mtl_renderer.h
        renderer/backends/metal/mtl_window.h
        renderer/backends/metal/mtl_pipeline.h
        renderer/backends/metal/mtl_texture.h
        renderer/backends/metal/mtl_command_buffer.h

        renderer/backends/metal/private/mtl_implementation.h
        renderer/backends/metal/private/mtl_renderer.mm
        renderer/backends/metal/private/mtl_window.mm
        renderer/backends/metal/private/mtl_pipeline.mm
        renderer/backends/metal/private/mtl_texture.mm
        renderer/backends/metal/private/mtl_command_buffer.mm

        # renderer backend: vulkan
        renderer/backends/vulkan/vk_renderer.h
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

add_library(engine_library ${SOURCES_LIBRARY})

set(SOURCES
        main.cpp
)

add_executable(bored_engine ${SOURCES})
target_link_libraries(bored_engine engine_library)