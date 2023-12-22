set(CGLTF_SOURCES
        cgltf/cgltf.h
        cgltf/cgltf_write.h
)

add_library(cgltf INTERFACE ${CGLTF_SOURCES})

target_include_directories(cgltf INTERFACE cgltf)