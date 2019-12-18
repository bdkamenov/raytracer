cmake_minimum_required(VERSION 3.12)
project(raytracer LANGUAGES CXX CUDA)
project(cuda_base CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

include_directories("/usr/local/cuda/include")
enable_language(CUDA)
set(CMAKE_CUDA_COMPILER /usr/bin/nvcc)
add_definitions(-D__CUDACC__)

add_library(cuda_base src/cuda/cuda.h src/cuda/cuda.cu)
target_compile_features(cuda_base PUBLIC cxx_std_11)

set_target_properties(cuda_base
        PROPERTIES CUDA_SEPARABLE_COMPILATION ON
        )
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -gencode arch=compute_35,code=sm_35")


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include_directories(src)

find_library(SDL2 REQUIRED SDL2)
find_library(OPENEXR_LIBS REQUIRED)

file(GLOB SOURCES
        src/*.h
        src/*.cpp
        src/color/*.h
        src/color/*.cpp
        src/geometries/*.h
        src/geometries/*.cpp
        src/materials/*.h
        src/materials/*.cpp
        src/maths/*.h
        src/maths/*.cpp
        src/render/*.h
        src/render/*.cpp
        src/scenes/*.h
        src/scenes/*.cpp
        src/shaders/*.h
        src/shaders/*.cpp
        src/utils/*.h
        src/utils/*.cpp
        )

add_executable(raytracer ${SOURCES})


set_target_properties(
        raytracer
        PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON
)

target_link_libraries(raytracer PRIVATE SDL2)
target_link_libraries(raytracer PRIVATE /usr/local/lib/libIex.so)
target_link_libraries(raytracer PRIVATE /usr/local/lib/libIlmThread.so)
target_link_libraries(raytracer PRIVATE /usr/local/lib/libIlmImfUtil.so)
target_link_libraries(raytracer PRIVATE /usr/local/lib/libIlmImf.so)
target_link_libraries(raytracer PRIVATE /usr/local/lib/libHalf.so)

target_link_libraries(raytracer PRIVATE cuda_base)