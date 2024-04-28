# project(editor)

set(EDITOR_SOURCES
        main.cpp
        ui.h
        ui.cpp
        editor.h
        editor.cpp
        camera_controller.h
        camera_controller.cpp
        axes.h
        axes.cpp
)

add_executable(editor ${EDITOR_SOURCES})
target_link_libraries(editor PRIVATE math graphics renderer asset entity input import_gltf import_texture)