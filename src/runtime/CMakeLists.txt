add_subdirectory(math)
add_subdirectory(graphics)
add_subdirectory(renderer)

add_library(runtime INTERFACE)
target_link_libraries(runtime INTERFACE math graphics renderer)