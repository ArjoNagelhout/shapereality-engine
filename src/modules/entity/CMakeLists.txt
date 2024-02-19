set(ENTITY_SOURCES
        config.h
        registry.h
        serialize.h
        sparse_set.h
        type.h
        view.h

        components/hierarchy.h
        components/hierarchy.cpp
)

add_library(entity ${ENTITY_SOURCES})
target_link_libraries(entity reflection)
target_include_directories(entity PUBLIC ..)