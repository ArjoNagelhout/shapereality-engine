set(RENDERER_SOURCES
        mesh.h
        mesh.cpp
)

add_library(renderer ${RENDERER_SOURCES})

target_include_directories(renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(renderer math graphics)