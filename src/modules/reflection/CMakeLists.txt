set(REFLECTION_SOURCES
        type_id.h
        type_info.h
        type_info.cpp
        class.h
        class.cpp
        enum.h
        enum.cpp
        type_registry.h
        type_registry.cpp
        serialize/json.h
        serialize/json.cpp
        serialize/binary.h
        serialize/binary.cpp
        reflection.h
        reflection.cpp
        any/any_unique_ptr.h
        any/any_unique_ptr.cpp
        handlers/any_unordered_map_handler.h
        handlers/any_vector_handler.h
        any/any.h
        any/any.cpp
        serialize/hybrid.h
        serialize/hybrid.cpp
)

add_library(reflection ${REFLECTION_SOURCES})
target_link_libraries(reflection json)
target_include_directories(reflection PUBLIC ..)