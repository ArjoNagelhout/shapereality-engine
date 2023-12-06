set(GRAPHICS_SOURCES
        # graphics
        graphics.h
        graphics.cpp
        device.h
        window.h
        window_cocoa.h
        window_cocoa.mm
        window.cpp
        texture.h
        command_buffer.h
        render_pass.h
        buffer.h

        # graphics backend: metal
        backends/metal/mtl_device.h
        backends/metal/mtl_window.h
        backends/metal/mtl_texture.h
        backends/metal/mtl_command_buffer.h

        backends/metal/mtl_window.mm
        backends/metal/mtl_texture.mm
        backends/metal/mtl_command_buffer.mm
        backends/metal/mtl_device.mm
        device.cpp
        shader_library.h
        command_queue.h
        backends/metal/mtl_command_queue.h
        backends/metal/mtl_command_queue.mm

        # graphics backend: vulkan
)

set_source_files_properties(graphics.cpp PROPERTIES
        COMPILE_FLAGS "-x objective-c++")

add_library(graphics ${GRAPHICS_SOURCES})