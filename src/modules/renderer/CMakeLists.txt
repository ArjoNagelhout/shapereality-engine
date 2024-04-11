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

        mesh_renderer.h

        transform.h
        transform.cpp

        imgui/imgui_backend.h
        imgui/imgui_backend.cpp

        scene_renderer.h
        scene_renderer.cpp

        render_graph.h
        render_graph.cpp

        register.h
        register.cpp
)

add_library(renderer ${RENDERING_SOURCES})

target_include_directories(renderer PUBLIC ..)

target_link_libraries(renderer reflection scene math graphics imgui)