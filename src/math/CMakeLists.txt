set(MATH_SOURCES
        bounds.h
        bounds.inl

        matrix.h
        matrix.inl

        plane.h
        plane.inl

        quaternion.h
        quaternion.inl

        ray.h
        ray.inl
        ray.cpp

        rect.h
        rect.cpp

        vector.h
        vector.inl
)

add_library(math ${MATH_SOURCES})

target_include_directories(math PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:math/>  # <prefix>/include/mylib
)