project(bored_engine_tests)

add_subdirectory(external/googletest)

set(SOURCES
        # math
        math/math_tests.cpp
)

add_executable(bored_engine_tests ${SOURCES})

target_include_directories(bored_engine_tests PUBLIC ${gtest_SOURCE_DIR}/include/gtest ${gtest_SOURCE_DIR})
target_link_libraries(bored_engine_tests engine_library gtest gtest_main)