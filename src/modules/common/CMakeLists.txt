set(COMMON_SOURCES
        log.h
        log.cpp
        result.h
        thread_pool.h
        thread_pool.cpp
)

add_library(common ${COMMON_SOURCES})

target_include_directories(common PUBLIC ..)

target_link_libraries(common thread_pool)