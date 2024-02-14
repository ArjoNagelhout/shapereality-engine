set(REFLECTION_SOURCES
        type_id.h
        type_info.h
        json.h
        json.cpp
)

add_library(reflection ${REFLECTION_SOURCES})
target_link_libraries(reflection json)
target_include_directories(reflection PUBLIC ..)