project(tests)

set(SOURCES
        # math
        math/math_tests.cpp
)

add_executable(tests_run ${SOURCES})

target_link_libraries(tests_run engine_library)