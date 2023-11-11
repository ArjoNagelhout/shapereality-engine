cmake_minimum_required(VERSION 3.26)
project(bored_engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(bored_engine main.cpp
        assets.h
        assets.cpp)
