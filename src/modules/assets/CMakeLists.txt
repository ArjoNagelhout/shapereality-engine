set(ASSETS_SOURCES
        import/gltf_importer.h
        import/gltf_importer.cpp
        import/texture_importer.h
        import/texture_importer.cpp

        source_directory.h
        source_directory.cpp
        importer_registry.h
        importer_registry.cpp
)

add_library(assets ${ASSETS_SOURCES})

target_include_directories(assets PUBLIC ..)

target_link_libraries(assets renderer cgltf lodepng json)