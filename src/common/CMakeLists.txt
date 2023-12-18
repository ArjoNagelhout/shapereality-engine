set(COMMON_SOURCES
        platform/cocoa/cocoa.h
        platform/cocoa/cocoa.mm
)

add_library(common ${COMMON_SOURCES})

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)