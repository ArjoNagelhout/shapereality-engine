set(REFLECTION_SOURCES
        reflection.h
        reflection.cpp
        type_id.h
)

add_library(reflection ${REFLECTION_SOURCES})

target_include_directories(reflection PUBLIC ..)