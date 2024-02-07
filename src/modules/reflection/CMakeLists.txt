set(REFLECTION_SOURCES
        type_id.h
        type_info.h
)

add_library(reflection INTERFACE ${REFLECTION_SOURCES})

target_include_directories(reflection INTERFACE ..)