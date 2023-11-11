add_subdirectory(yaml-cpp)

# ---------- Metal -----------------

add_library(metal INTERFACE)
target_include_directories(metal INTERFACE metal-cpp metal-cpp-extensions)
target_link_libraries(metal INTERFACE
        "-framework Metal"
        "-framework MetalKit"
        "-framework AppKit"
        "-framework Foundation"
        "-framework QuartzCore"
)