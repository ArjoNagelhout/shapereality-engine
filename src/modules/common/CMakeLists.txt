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

        platform/apple/apple_application_info.mm
        platform/apple/apple.h
        platform/apple/apple.mm
)

add_library(common ${COMMON_SOURCES})

target_include_directories(common PUBLIC ..)

target_link_libraries(common thread_pool fmt)

# todo: only do this on macOS
#set_source_files_properties(application_info.cpp PROPERTIES
#        COMPILE_FLAGS "-x objective-c++")

# ---------- link Apple frameworks -----------------

target_link_libraries(common
        "-framework Cocoa"
        "-framework Foundation"
)