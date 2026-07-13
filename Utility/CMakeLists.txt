cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Utility)

file(GLOB_RECURSE SOURCES src/*.cpp src/IPC/*.cpp include/Utility/*.hpp include/Utility/IPC/*.hpp include/Utility/Private/*.inl)
add_library(utility_lib STATIC ${SOURCES})

target_compile_options(utility_lib PRIVATE -O3)
target_include_directories(utility_lib PUBLIC include)
