set(RENDERER_SOURCES
        mesh.h
        mesh.cpp
        camera.h
        camera.cpp

        scene/scene.h
        text/text.h
        ui/ui.h

        shader.h
        shader.cpp

        material.h
        material.cpp

        mesh_renderer.h

        transform.h
        transform.cpp

        imgui_implementation.h
        imgui_implementation.cpp
)

add_library(renderer ${RENDERER_SOURCES})

target_include_directories(renderer PUBLIC ..)

target_link_libraries(renderer math graphics imgui)