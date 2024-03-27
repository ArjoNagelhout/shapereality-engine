set(GRAPHICS_SOURCES
        types.h

        application.h
        application.cpp

        input.h
        input.cpp

        window.h
        window.cpp

        device.h

        command_buffer.h
        command_queue.h
        render_pass.h
        render_pipeline_state.h
        buffer.h
        texture.h
        shader.h

        # platform: cocoa (macOS)
        platform/cocoa/cocoa.h
        platform/cocoa/cocoa.mm
        platform/cocoa/cocoa_application.mm
        platform/cocoa/cocoa_window.h
        platform/cocoa/cocoa_window.mm
        platform/cocoa/cocoa_input.h
        platform/cocoa/cocoa_input.mm

        # platform: uikit (iOS)
        platform/uikit/uikit.h
        platform/uikit/uikit_window.h
        platform/uikit/uikit_window.mm

        # graphics backend: metal
        backends/metal/mtl_types.h
        backends/metal/mtl_types.mm

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

        backends/metal/mtl_shader.h
        backends/metal/mtl_shader.mm

        backends/metal/mtl_utils.h
        backends/metal/mtl_utils.mm

        backends/metal/mtl_buffer.h
        backends/metal/mtl_buffer.mm

        # graphics backend: vulkan
        backends/vulkan/vk_device.h
        backends/vulkan/vk_device.cpp

)

add_library(graphics ${GRAPHICS_SOURCES})

# todo: only do this on macOS
set_source_files_properties(graphics.cpp PROPERTIES
        COMPILE_FLAGS "-x objective-c++")

# ---------- link Apple frameworks -----------------

target_link_libraries(graphics
        "-framework Cocoa"
        "-framework Foundation"
        "-framework AppKit"
        "-framework Metal"
        "-framework MetalKit"
)

target_include_directories(graphics PUBLIC ..)