set(ENTITY_SOURCES
        sparse_set.h
)

add_library(entity INTERFACE ${ENTITY_SOURCES})

target_include_directories(entity INTERFACE ..)