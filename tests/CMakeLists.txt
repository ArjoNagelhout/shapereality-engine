project(bored_engine_tests)

add_subdirectory(external/googletest)

set(SOURCES
        # math
        math/vector.cpp
        math/quaternion.cpp
        math/ray.cpp
        math/plane.cpp
        math/matrix.cpp
        math/bounds.cpp
        scratch_pad.cpp
)

add_executable(bored_engine_tests ${SOURCES})

target_link_libraries(bored_engine_tests bored_engine_library gtest gtest_main)