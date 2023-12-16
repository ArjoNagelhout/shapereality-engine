set(GRAPHICS_SOURCES
        graphics.h
        graphics.cpp

        types.h
        types.cpp

        window.h
        window_cocoa.h
        window_cocoa.mm
        window.cpp

        device.h
        device.cpp

        command_buffer.h
        command_queue.h
        render_pass.h
        render_pipeline_state.h

        buffer.h
        texture.h

        # graphics backend: metal
        backends/metal/mtl_types.h
        backends/metal/mtl_types.mm

        backends/metal/mtl_window.h
        backends/metal/mtl_window.mm

        backends/metal/mtl_device.h
        backends/metal/mtl_device.mm

        backends/metal/mtl_command_buffer.h
        backends/metal/mtl_command_buffer.mm

        backends/metal/mtl_command_queue.h
        backends/metal/mtl_command_queue.mm

        backends/metal/mtl_render_pass.h
        backends/metal/mtl_render_pass.mm

        backends/metal/mtl_render_pipeline_state.h
        backends/metal/mtl_render_pipeline_state.mm

        backends/metal/mtl_texture.h
        backends/metal/mtl_texture.mm
        backends/vulkan/vk_device.h
        backends/vulkan/vk_device.cpp
        shader.h
        backends/metal/mtl_shader.h
        backends/metal/mtl_shader.mm
        shader.cpp
        backends/metal/mtl_utils.h
        backends/metal/mtl_utils.mm
        backends/metal/mtl_buffer.h
        backends/metal/mtl_buffer.mm

        # graphics backend: vulkan
)

set_source_files_properties(graphics.cpp PROPERTIES
        COMPILE_FLAGS "-x objective-c++")

add_library(graphics ${GRAPHICS_SOURCES})

target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..) # todo improve using something like graphics/include/graphics