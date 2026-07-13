cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Consumer)

file(GLOB_RECURSE SOURCES src/*.cpp include/Consumer/*.hpp include/Consumer/Private/*.inl)
add_executable(consumer ${SOURCES})

target_link_libraries(consumer PRIVATE utility_lib)
target_include_directories(consumer PRIVATE include)

install(TARGETS consumer DESTINATION bin)
