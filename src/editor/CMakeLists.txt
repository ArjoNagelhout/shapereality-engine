# project(editor)

set(EDITOR_SOURCES
        main.cpp
        ui.h
        ui.cpp
        handles/translate_handle.h
)

add_executable(editor ${EDITOR_SOURCES})
target_link_libraries(editor PRIVATE math graphics rendering assets entity input)