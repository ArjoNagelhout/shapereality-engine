# project(editor)

set(EDITOR_SOURCES
        main.cpp
        ui.h
        ui.cpp
)

add_executable(editor ${EDITOR_SOURCES})
target_link_libraries(editor PRIVATE math graphics renderer assets entity input)