cmake_minimum_required(VERSION 3.14)
project(UAVNavigationUncertainty VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Strict warnings for research-grade code
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror=return-type")

# Include directories
include_directories(include)

# ---------------------------------------------------------
# Phase 1: Grid Library
# ---------------------------------------------------------
add_library(grid src/grid/OccupancyGrid.cpp)
target_include_directories(grid PUBLIC include)

# ---------------------------------------------------------
# Testing via GoogleTest
# ---------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_grid tests/test_grid.cpp)
target_link_libraries(test_grid PRIVATE grid gtest_main)

include(GoogleTest)
gtest_discover_tests(test_grid)