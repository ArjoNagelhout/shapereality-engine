set(TEXTURE_SOURCES
        texture.h
        texture.cpp
        png.h
        png.cpp
        register.h
        register.cpp
)

add_library(import_texture ${TEXTURE_SOURCES})

target_include_directories(import_texture PUBLIC ..)

target_link_libraries(import_texture asset renderer lodepng)