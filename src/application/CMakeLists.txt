set(APPLICATION_SOURCES
        main.cpp
        ../assets_test/main.cpp
)

add_executable(application ${APPLICATION_SOURCES})
target_link_libraries(application PRIVATE math graphics renderer assets entity)