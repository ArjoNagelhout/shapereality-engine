project(bored_engine_library)

# ---------- create engine library -----------------

set(SOURCES_LIBRARY
        # core
        types.h
        application.h
        application.cpp
        application_cocoa.mm

        # math
        math/rect.h
        math/rect.cpp
        math/vector.h
        math/vector.inl
        math/matrix.h
        math/quaternion.h
        math/quaternion.inl
        math/plane.h
        math/bounds.h
        math/bounds.inl
        math/ray.h
        math/ray.inl
        math/ray.cpp
        math/matrix.inl
        math/plane.inl

        # scene
        scene/scene.h
        scene/scene.cpp
)

add_library(bored_engine_library ${SOURCES_LIBRARY})
target_include_directories(bored_engine_library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# ---------- link internal modules -----------------

add_subdirectory(graphics)
target_link_libraries(bored_engine_library graphics)

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
        ../data/shader/shader.metal
)

add_executable(bored_engine ${SOURCES})
target_link_libraries(bored_engine bored_engine_library)