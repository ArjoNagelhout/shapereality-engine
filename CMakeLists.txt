cmake_minimum_required(VERSION 3.26)
project(bored_engine)
set(CMAKE_CXX_STANDARD 20)

# ---------- Objective-C support -----------
# https://discourse.cmake.org/t/configure-cmake-to-only-use-objc-linker-only-on-macos-with-pch/3795/4

if (APPLE)
    enable_language(OBJC)
endif ()

# ---------- create target -----------------

set(SOURCES
        src/main.cpp
        src/renderer/renderer.h
        src/renderer/renderer.cpp
        src/renderer/backends/metal/metal_renderer.h
        src/renderer/backends/metal/metal_renderer.cpp
        src/renderer/backends/vulkan/vk_renderer.h
        src/renderer/backends/vulkan/vk_renderer.cpp
        src/application.h
        src/application.cpp
        src/window.h
        src/macos_window.cpp
        src/window_test.mm
        src/window_test.h
)

add_executable(bored_engine ${SOURCES})

# ---------- add subdirectories -------------

add_subdirectory(external)
#add_subdirectory(libs)
add_subdirectory(src)

# ---- Apple specific fun
#set_source_files_properties(src/main.cpp PROPERTIES
#        COMPILE_FLAGS "-x objective-c++")

target_link_libraries(bored_engine "-framework Cocoa" "-framework Foundation" "-framework AppKit")



target_link_libraries(bored_engine yaml-cpp metal)