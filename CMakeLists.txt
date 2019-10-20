cmake_minimum_required(VERSION 3.12)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include_directories(src)

find_library(SDL2 REQUIRED SDL2)

file(GLOB SOURCES
        src/*.cpp
        src/color/*.cpp
        src/geometries/*.cpp
        src/maths/*.cpp
        src/render/*.cpp
        src/scenes/*.cpp
        src/utils/*.cpp
        src/shaders/*.cpp
        )

add_executable(raytracer ${SOURCES})

target_link_libraries(raytracer PRIVATE SDL2)