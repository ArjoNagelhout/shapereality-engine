set(ASSET_SOURCES
        asset_id.h
        asset_id.cpp

        import_registry.h
        import_registry.cpp

        asset_database.h
        asset_database.cpp

        asset_handle.h
        asset_handle.cpp

        reflection.h
        reflection.cpp
        asset_info.h
)

add_library(asset ${ASSET_SOURCES})

target_include_directories(asset PUBLIC ..)

target_link_libraries(asset common reflection thread_pool date json)