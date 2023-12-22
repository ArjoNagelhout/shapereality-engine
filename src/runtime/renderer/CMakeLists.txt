set(RENDERER_SOURCES
        mesh.h
        mesh.cpp
        camera.h
        camera.cpp
        transform.h
        scene/scene.h
        text/text.h
        ui/ui.h
)

add_library(renderer ${RENDERER_SOURCES})

target_include_directories(renderer PUBLIC ..)

target_link_libraries(renderer math graphics)