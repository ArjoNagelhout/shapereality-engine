set(SCENE_SOURCES
        scene.h
        scene.cpp
)

add_library(scene ${SCENE_SOURCES})

target_include_directories(scene PUBLIC ..)

target_link_libraries(scene entity)