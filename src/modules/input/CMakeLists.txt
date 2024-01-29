set(INPUT_SOURCES
        input.h
        input.cpp
)

add_library(input ${INPUT_SOURCES})

target_include_directories(input PUBLIC ..)

target_link_libraries(input graphics)