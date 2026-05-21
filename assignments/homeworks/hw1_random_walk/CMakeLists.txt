# EC327 Summer 2026 -- Homework 1 build configuration.
#
# Build and run from this directory with:
#     cmake -B build
#     cmake --build build
#     ./build/hw1_main
#
# You may extend this file if you add source files of your own.

cmake_minimum_required(VERSION 3.16)
project(hw1_random_walk CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Your code must compile cleanly with these warnings turned on.
add_compile_options(-Wall -Wextra)

add_executable(hw1_main
  hw1_main.cpp
  Walker.cpp
  Histogram.cpp
)
