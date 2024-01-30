set(RENDERING_SOURCES
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

        imgui/imgui_backend.h
        imgui/imgui_backend.cpp

        scene_renderer.h
        scene_renderer.cpp

        render_graph.h
        render_graph.cpp
)

add_library(rendering ${RENDERING_SOURCES})

target_include_directories(rendering PUBLIC ..)

target_link_libraries(rendering scene math graphics imgui)