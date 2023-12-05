set(GRAPHICS_SOURCES
        # graphics
        graphics.h
        graphics.cpp
        window.h
        window_cocoa.h
        window_cocoa.mm
        window.cpp
        pipeline.h
        pipeline.cpp
        shader.h
        shader.cpp
        material.h
        material.cpp
        mesh.h
        mesh.cpp
        texture.h
        texture.cpp
        command_buffer.h
        command_buffer.cpp
        render_pass.h
        render_pass.cpp
        buffer.h
        buffer.cpp

        # graphics backend: metal
        backends/metal/mtl_graphics.h
        backends/metal/mtl_window.h
        backends/metal/mtl_pipeline.h
        backends/metal/mtl_texture.h
        backends/metal/mtl_command_buffer.h

        backends/metal/private/mtl_implementation.h
        backends/metal/private/mtl_graphics.mm
        backends/metal/private/mtl_window.mm
        backends/metal/private/mtl_pipeline.mm
        backends/metal/private/mtl_texture.mm
        backends/metal/private/mtl_command_buffer.mm

        # graphics backend: vulkan
        backends/vulkan/vk_graphics.h
        backends/vulkan/vk_window.h
        backends/vulkan/vk_pipeline.h
        backends/vulkan/vk_texture.h
        backends/vulkan/vk_command_buffer.h

        backends/vulkan/private/vk_renderer.cpp
        backends/vulkan/private/vk_window.cpp
        backends/vulkan/private/vk_pipeline.cpp
        backends/vulkan/private/vk_texture.cpp
        backends/vulkan/private/vk_command_buffer.cpp
)

add_library(graphics ${GRAPHICS_SOURCES})