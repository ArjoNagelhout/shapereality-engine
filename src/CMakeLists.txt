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

# ---------- link internal libraries -----------------

add_subdirectory(math)
add_subdirectory(graphics)
add_subdirectory(renderer)

target_link_libraries(bored_engine_library math graphics renderer)

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