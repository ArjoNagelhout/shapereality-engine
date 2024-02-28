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

        #asset
        asset/async.cpp

        #reflection
        reflection/graph_based_reflection_json.cpp
        reflection/enum.cpp
        asset/thread_pool.cpp
        asset/thread_pool_v2.cpp
)

target_link_libraries(shapereality_test thread_pool entity math reflection json gtest gtest_main)