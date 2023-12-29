add_subdirectory(runtime)
add_subdirectory(editor)
add_subdirectory(application)

add_executable(transform_ecs_test transform_ecs_test.cpp
        runtime/core/error.h
        runtime/core/error.cpp)
target_link_libraries(transform_ecs_test math)