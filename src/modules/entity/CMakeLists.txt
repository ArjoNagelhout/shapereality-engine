set(ENTITY_SOURCES
        config.h
        entity_registry.h
        serialize.h
        sparse_set.h
        type.h
        view.h

        components/hierarchy.h
        components/hierarchy.cpp
        register.h
        register.cpp
)

add_library(entity ${ENTITY_SOURCES})
target_link_libraries(entity reflection)
target_include_directories(entity PUBLIC ..)