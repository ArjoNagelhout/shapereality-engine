# ---------- runtime library -----------------
add_subdirectory(runtime)

# ---------- create executable -----------------

set(SOURCES
        main.cpp
)

add_executable(bored_engine ${SOURCES})
target_link_libraries(bored_engine PRIVATE runtime)