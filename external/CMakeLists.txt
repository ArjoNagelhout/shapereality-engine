# ------------------- CGLTF (gltf import / export library) -------------------------------
set(CGLTF_SOURCES
        cgltf/cgltf.h
        cgltf/cgltf_write.h
        ../src/transform_ecs_test.cpp
)

add_library(cgltf INTERFACE ${CGLTF_SOURCES})
target_include_directories(cgltf INTERFACE cgltf)

# ------------------- LodePNG (png import / export library) -------------------------------
set(LODEPNG_SOURCES
        lodepng/lodepng.h
        lodepng/lodepng.cpp
)

add_library(lodepng ${LODEPNG_SOURCES})
target_include_directories(lodepng PUBLIC lodepng)