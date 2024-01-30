set(RENDERER_SOURCES
        mesh.h
        mesh.cpp

        camera.h
        camera.cpp

        text/text.h
        ui/ui.h

        shader.h
        shader.cpp

        material.h
        material.cpp

        mesh_renderer.h

        transform.h
        transform.cpp

        imgui_backend.h
        imgui_backend.cpp

        scene.h
        scene.cpp

        render_graph.h
        render_graph.cpp
)

add_library(renderer ${RENDERER_SOURCES})

target_include_directories(renderer PUBLIC ..)

target_link_libraries(renderer math graphics imgui)