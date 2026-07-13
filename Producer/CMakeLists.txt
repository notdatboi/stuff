cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Producer)

file(GLOB_RECURSE SOURCES src/*.cpp include/Producer/*.hpp include/Producer/Private/*.inl)
add_executable(producer ${SOURCES})

target_link_libraries(producer PRIVATE utility_lib)
target_include_directories(producer PRIVATE include)

install(TARGETS producer DESTINATION bin)
