set(APPLICATION_SOURCES
        main.cpp
)

add_executable(application ${APPLICATION_SOURCES})
target_link_libraries(application PRIVATE math graphics renderer assets entity)

add_executable(huilen segfault.cpp)