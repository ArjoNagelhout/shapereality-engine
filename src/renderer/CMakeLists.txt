set(RENDERER_SOURCES
        mesh.h
        mesh.cpp
)

add_library(renderer ${RENDERER_SOURCES})

target_link_libraries(renderer math graphics)