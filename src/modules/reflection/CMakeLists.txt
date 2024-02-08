set(REFLECTION_SOURCES
        type_id.h
        type_info.h
        type_info.cpp
)

add_library(reflection ${REFLECTION_SOURCES})
target_link_libraries(reflection INTERFACE rapidjson)
target_include_directories(reflection PUBLIC ..)