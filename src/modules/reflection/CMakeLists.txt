set(REFLECTION_SOURCES
        type_id.h
        type_info.h
        type_info.cpp
        class.h
        class.cpp
        enum.h
        enum.cpp
        type_info_registry.h
        type_info_registry.cpp
        reflection_friend.h
        serialize/json.h
        serialize/json.cpp
        serialize/binary.h
        serialize/binary.cpp
        serialize/enum.h
        serialize/enum.cpp
)

add_library(reflection ${REFLECTION_SOURCES})
target_link_libraries(reflection json)
target_include_directories(reflection PUBLIC ..)