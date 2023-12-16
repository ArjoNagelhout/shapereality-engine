project(bored_engine_library)

# ---------- create engine library -----------------

set(SOURCES_LIBRARY
        # core
        types.h
        application.h
        application.cpp
        application_cocoa.mm
)

add_library(bored_engine_library ${SOURCES_LIBRARY})
target_include_directories(bored_engine_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# ---------- link internal modules -----------------

add_subdirectory(graphics)
add_subdirectory(math)
add_subdirectory(renderer)

target_link_libraries(bored_engine_library graphics math renderer)

# ---------- link Apple frameworks -----------------

target_link_libraries(bored_engine_library
        "-framework Cocoa"
        "-framework Foundation"
        "-framework AppKit"
        "-framework Metal"
        "-framework MetalKit"
)

# ---------- create executable -----------------

set(SOURCES
        main.cpp
)

add_executable(bored_engine ${SOURCES})
target_link_libraries(bored_engine bored_engine_library)