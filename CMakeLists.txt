cmake_minimum_required(VERSION 3.16)
project(conjunction_detector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

add_library(conjunction_core
    src/core.cpp
    src/ingest/omm.cpp
)

target_include_directories(conjunction_core PUBLIC src)
target_link_libraries(conjunction_core PUBLIC SGP4::sgp4)

add_executable(conjunction src/cli/main.cpp)
target_link_libraries(conjunction PRIVATE conjunction_core)

include(FetchContent)

FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1)

FetchContent_Declare(sgp4
    GIT_REPOSITORY https://github.com/dnwrnr/sgp4.git
    GIT_TAG c6ebb90625b43d7f6c6eb38b386f343d3cfe03a6)

FetchContent_MakeAvailable(Catch2)
FetchContent_MakeAvailable(sgp4)

enable_testing()
add_executable(unit_tests tests/test_smoke.cpp)
target_link_libraries(unit_tests PRIVATE conjunction_core Catch2::Catch2WithMain)
add_test(NAME unit_tests COMMAND unit_tests)
