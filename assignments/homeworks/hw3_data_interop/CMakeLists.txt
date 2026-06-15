# EC327 Summer 2026 -- Homework 3 build configuration.
#
# Build and run from this directory with:
#     cmake -B build
#     cmake --build build
#     ./build/hw3_main
#
# You may extend this file if you add source files of your own.

cmake_minimum_required(VERSION 3.16)
project(hw3_data_interop CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Your code must compile cleanly with these warnings turned on.
add_compile_options(-Wall -Wextra)

add_executable(hw3_main
  hw3_main.cpp
  DataSet.cpp
  Walker2D.cpp
  csv_writer.cpp
  npy_writer.cpp
)
