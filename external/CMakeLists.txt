# ------------------------------------
# CGLTF (gltf import / export library)
# ------------------------------------
set(CGLTF_SOURCES
        cgltf/cgltf.h
        cgltf/cgltf_write.h
        ../data/shaders/new.metal
)

add_library(cgltf INTERFACE ${CGLTF_SOURCES})
target_include_directories(cgltf INTERFACE cgltf)

# ---------------------------------
# Date library, (because Clang does
# not support std::format yet)
# ---------------------------------
set(DATE_SOURCES
        date/include/date/date.h
)

add_library(date INTERFACE)
target_include_directories(date INTERFACE date/include)

# ---------------------------------
# fmt (because Apple Clang does not
# support std::format...)
# ---------------------------------

add_subdirectory(fmt)

# ----------------------------
# ImGui (temporary UI library)
# ----------------------------
add_library(imgui STATIC)
target_sources(imgui PUBLIC
        imgui/imgui.h
        imgui/imgui.cpp

        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_widgets.cpp

        imgui/imgui_demo.cpp
        imgui/imgui_tables.cpp

        imgui/backends/imgui_impl_metal.h
        imgui/backends/imgui_impl_metal.mm

        imgui/misc/cpp/imgui_stdlib.h
        imgui/misc/cpp/imgui_stdlib.cpp
)
target_include_directories(imgui PUBLIC imgui imgui/backends)

# -------------------
# Json for Modern C++
# -------------------
set(JSON_SOURCES
        json/include/nlohmann/json.hpp
)

add_library(json INTERFACE ${JSON_SOURCES})
target_include_directories(json INTERFACE json/include)

# -------------------------------------
# LodePNG (png import / export library)
# -------------------------------------
set(LODEPNG_SOURCES
        lodepng/lodepng.h
        lodepng/lodepng.cpp
)

add_library(lodepng ${LODEPNG_SOURCES})
target_include_directories(lodepng PUBLIC lodepng)

# -----------
# Thread pool
# -----------
set(THREAD_POOL_SOURCES
        thread-pool/include/BS_thread_pool.hpp
)

add_library(thread_pool INTERFACE ${THREAD_POOL_SOURCES})
target_include_directories(thread_pool INTERFACE thread-pool/include)