set(ASSETS_SOURCES
        import/gltf_importer.h
        import/gltf_importer.cpp
        import/texture_importer.h
        import/texture_importer.cpp

        assets_source_directory.h
        assets_source_directory.cpp
)

add_library(assets ${ASSETS_SOURCES})

target_include_directories(assets PUBLIC ..)

target_link_libraries(assets renderer cgltf lodepng json)

add_executable(assets_testing assets_testing.cpp)
target_link_libraries(assets_testing assets)