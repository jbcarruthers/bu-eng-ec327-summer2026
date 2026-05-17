# Minimal CMakeLists.txt — Lab 1 CMake cameo.
# Builds `poem` from `poem.cpp`. Lab 2 picks this up and extends it.
cmake_minimum_required(VERSION 3.20)
project(lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

add_executable(poem poem.cpp)
