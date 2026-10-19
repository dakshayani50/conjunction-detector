cmake_minimum_required(VERSION 3.16)
project(conjunction_detector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

include(FetchContent)

# ------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------

FetchContent_Declare(
    sgp4
    GIT_REPOSITORY https://github.com/dnwrnr/sgp4.git
    GIT_TAG c6ebb90625b43d7f6c6eb38b386f343d3cfe03a6
)

FetchContent_MakeAvailable(sgp4)

# ------------------------------------------------------------
# Core library
# ------------------------------------------------------------

add_library(conjunction_core
    src/core.cpp
    src/ingest/omm.cpp
    src/propagation/sgp4_propagator.cpp
)

target_include_directories(conjunction_core
    PUBLIC
        src
)

target_include_directories(conjunction_core
    PRIVATE
        ${sgp4_SOURCE_DIR}/libsgp4
)

target_link_libraries(conjunction_core
    PUBLIC
        SGP4::sgp4
)

# ------------------------------------------------------------
# CLI executable
# ------------------------------------------------------------

add_executable(conjunction
    src/cli/main.cpp
)

target_link_libraries(conjunction
    PRIVATE
        conjunction_core
)

# ------------------------------------------------------------
# Tests
# ------------------------------------------------------------

enable_testing()

find_package(Catch2 3 QUIET)

if(Catch2_FOUND)
    add_executable(unit_tests
        tests/test_smoke.cpp
    )

    target_link_libraries(unit_tests
        PRIVATE
            conjunction_core
            Catch2::Catch2WithMain
    )

    add_test(NAME unit_tests COMMAND unit_tests)
else()
    message(STATUS "Catch2 not found - skipping unit_tests target")
endif()