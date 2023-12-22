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
        utils.h
        utils.cpp
)

add_library(math ${MATH_SOURCES})

target_include_directories(math PUBLIC ..)