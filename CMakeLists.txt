cmake_minimum_required(VERSION 3.16)
project(conjunction_detector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(conjunction src/main.cpp)
