set(COMMON_SOURCES
        log.h
        log.cpp
        result.h
        promise.h
)

add_library(common ${COMMON_SOURCES})

target_include_directories(common PUBLIC ..)