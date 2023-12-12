set(GRAPHICS_SOURCES
        # graphics
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

        # graphics backend: vulkan
)

set_source_files_properties(graphics.cpp PROPERTIES
        COMPILE_FLAGS "-x objective-c++")

add_library(graphics ${GRAPHICS_SOURCES})