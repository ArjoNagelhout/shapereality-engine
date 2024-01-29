# project(editor)

set(EDITOR_SOURCES
        main.cpp
        editor_ui.h
        editor_ui.cpp
        translate_handle.h
)

add_executable(editor ${EDITOR_SOURCES})
target_link_libraries(editor PRIVATE math graphics renderer assets entity)