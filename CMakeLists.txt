cmake_minimum_required(VERSION 3.12)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

file(GLOB SOURCES
        src/*.cpp
        src/color/*.cpp
        src/geometries/*.cpp
        src/maths/*.cpp
        src/render/*.cpp
        src/scenes/*.cpp
        src/utils/*.cpp
        )

add_executable(raytracer ${SOURCES})

INCLUDE(FindPkgConfig)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})