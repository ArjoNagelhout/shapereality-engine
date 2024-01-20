set(ENTITY_SOURCES
        config.h
        sparse_set.h
        registry.h
        type.h
        view.h

        components/hierarchy.h
        components/hierarchy.cpp
)

add_library(entity ${ENTITY_SOURCES})

target_include_directories(entity PUBLIC ..)