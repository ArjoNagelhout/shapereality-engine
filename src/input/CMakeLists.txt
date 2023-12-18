set(INPUT_SOURCES
        input.h
        input.cpp
        input_cocoa.mm
        types.h
        types_cocoa.mm
        types_cocoa.h
)

add_library(input ${INPUT_SOURCES})

target_link_libraries(input common)

target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)