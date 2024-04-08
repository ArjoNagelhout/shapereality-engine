set(GLTF_SOURCES
        gltf.h
        gltf.cpp
)

add_library(import_gltf ${GLTF_SOURCES})

target_include_directories(import_gltf PUBLIC ..)

target_link_libraries(import_gltf asset renderer cgltf)