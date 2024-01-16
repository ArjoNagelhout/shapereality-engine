set(COMMON_SOURCES
        error.h
        error.cpp
)

add_library(common ${COMMON_SOURCES})

target_include_directories(common PUBLIC ..)