set(ASSETS_SOURCES
        import/gltf_importer.h
        import/gltf_importer.cpp
        import/texture_importer.h
        import/texture_importer.cpp
)

add_library(assets ${ASSETS_SOURCES})

target_include_directories(assets PUBLIC ../../editor)

target_link_libraries(assets renderer cgltf lodepng)