set(INPUT_SOURCES
        input.h
        input.cpp
        input_cocoa.mm
        types.h
        types_cocoa.cpp
)

add_library(input ${INPUT_SOURCES})

target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)