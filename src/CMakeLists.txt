add_subdirectory(runtime)
add_subdirectory(editor)
add_subdirectory(application)

# temporary testing weee
add_executable(transform_ecs_test transform_ecs_test.cpp)
target_link_libraries(transform_ecs_test math entity)