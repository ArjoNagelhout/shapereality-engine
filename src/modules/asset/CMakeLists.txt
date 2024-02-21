set(ASSET_SOURCES
        import/gltf.h
        import/gltf.cpp
        import/texture.h
        import/texture.cpp
        import/import_registry.h
        import/import_registry.cpp

        asset_database.h
        asset_database.cpp
)

add_library(asset ${ASSET_SOURCES})

target_include_directories(asset PUBLIC ..)

target_link_libraries(asset renderer cgltf lodepng json)