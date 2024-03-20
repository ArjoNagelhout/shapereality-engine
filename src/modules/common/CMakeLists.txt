set(COMMON_SOURCES
        logger.h
        logger.cpp
        result.h
        thread_pool.h
        thread_pool.cpp
        observers.h
        result.cpp
        application_info.h
        application_info.cpp
)

add_library(common ${COMMON_SOURCES})

target_include_directories(common PUBLIC ..)

target_link_libraries(common thread_pool fmt)