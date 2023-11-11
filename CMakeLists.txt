cmake_minimum_required(VERSION 3.26)
project(bored_engine)
set(CMAKE_CXX_STANDARD 20)

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
        src/platform/platform.h
        src/platform/ios.h
        src/platform/macos.h
        src/platform/windows.h
        src/platform/android.h
        src/platform/unix.h
        src/window.h
)

add_executable(bored_engine ${SOURCES})

# ---------- add subdirectories -------------

add_subdirectory(external)
#add_subdirectory(libs)
add_subdirectory(src)

target_link_libraries(bored_engine yaml-cpp metal)