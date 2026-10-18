cmake_minimum_required(VERSION 3.16)
project(conjunction_detector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

add_library(conjunction_core src/core.cpp)
target_include_directories(conjunction_core PUBLIC src)

add_executable(conjunction src/cli/main.cpp)
target_link_libraries(conjunction PRIVATE conjunction_core)
