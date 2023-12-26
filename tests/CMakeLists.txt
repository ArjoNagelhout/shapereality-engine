project(shapereality_tests)

add_subdirectory(external/googletest)

set(SOURCES
        # math
        math/vector.cpp
        math/quaternion.cpp
        math/ray.cpp
        math/plane.cpp
        math/matrix.cpp
        math/bounds.cpp
        math/test.cpp
        math/neneng.cpp
)

add_executable(shapereality_tests ${SOURCES})

target_link_libraries(shapereality_tests math graphics renderer gtest gtest_main)