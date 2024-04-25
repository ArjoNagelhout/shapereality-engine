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

        rect.h

        vector.h
        vector.inl
        utility.h
        utility.cpp
        config.h
)

add_library(math ${MATH_SOURCES})

target_include_directories(math PUBLIC ..)