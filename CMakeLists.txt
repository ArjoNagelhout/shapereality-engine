cmake_minimum_required(VERSION 3.26)
project(bored_engine)
set(CMAKE_CXX_STANDARD 20)

# ---------- Objective-C support -----------
# https://discourse.cmake.org/t/configure-cmake-to-only-use-objc-linker-only-on-macos-with-pch/3795/4

if (APPLE)
    enable_language(OBJC)
endif ()

# ---------- add subdirectories -------------

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(external)
#add_subdirectory(libs)

# todo: should be removed
set_source_files_properties(src/main.cpp PROPERTIES
        COMPILE_FLAGS "-x objective-c++")

# link libraries
target_link_libraries(bored_engine "-framework Cocoa" "-framework Foundation" "-framework AppKit")
target_link_libraries(bored_engine yaml-cpp metal)