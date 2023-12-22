# project(editor)

add_subdirectory(assets)

set(EDITOR_SOURCES
        main.cpp
)

add_executable(editor ${EDITOR_SOURCES} assets)