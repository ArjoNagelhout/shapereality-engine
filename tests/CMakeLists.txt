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

        #reflection
        reflection/type_info.cpp
        reflection/vector.cpp
        reflection/json.cpp
        reflection/vector_prototype2.cpp
        reflection/graph_based_reflection.cpp
        reflection/graph_based_reflection_values.cpp
        reflection/graph_based_reflection_json.cpp
)

target_link_libraries(shapereality_test entity math reflection json gtest gtest_main)