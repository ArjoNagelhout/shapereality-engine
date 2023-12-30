set(ENTITY_SOURCES
        config.h
        sparse_set.h
        registry.h
)

add_library(entity INTERFACE ${ENTITY_SOURCES})

target_include_directories(entity INTERFACE ..)