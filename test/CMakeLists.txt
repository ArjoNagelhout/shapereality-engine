project(shapereality_test)

add_subdirectory(external/googletest)

add_executable(shapereality_test
        #entity
        entity/hierarchy.cpp
        entity/registry.cpp
        entity/view.cpp
        entity/sparse_set.cpp

        #math
        math/bounds.cpp
        math/matrix.cpp
        math/plane.cpp
        math/quaternion.cpp
        math/ray.cpp
        math/vector.cpp
)

target_link_libraries(shapereality_test entity math gtest gtest_main)