set(ASSETS_SOURCES
        import/mesh_importer.h
        import/mesh_importer.cpp
        import/texture_importer.h
        import/texture_importer.cpp
)

add_library(assets ${ASSETS_SOURCES})

target_include_directories(assets PUBLIC ..)

target_link_libraries(assets renderer cgltf)