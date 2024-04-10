set(ASSET_SOURCES
        asset_id.h
        asset_id.cpp

        import_registry.h
        import_registry.cpp

        asset_database.h
        asset_database.cpp

        asset_handle.h
        asset_handle.cpp

        register.h
        register.cpp
        asset_type.h
        asset_type_registry.h
        asset_type_registry.cpp
        asset_type.cpp
)

add_library(asset ${ASSET_SOURCES})

target_include_directories(asset PUBLIC ..)

target_link_libraries(asset common reflection thread_pool date json)