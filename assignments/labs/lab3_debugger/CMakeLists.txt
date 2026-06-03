# CMakeLists.txt — Lab 3 (GDB). Builds all four bug-hunt programs.
#
# The point of this file for Lab 3 is the build TYPE. GDB needs debug symbols
# (the `-g` flag); the `Debug` configuration adds them for you, so you don't
# have to remember `-g` on four separate g++ lines.
#
#   cmake -B build -DCMAKE_BUILD_TYPE=Debug
#   cmake --build build
#   gdb build/calclog | tee debug_calclog.txt
#
# (Building by hand with `g++ -std=c++20 -g <file>.cpp -o <file>` is equally
# fine — the lab shows both. Either way the binaries carry symbols.)
cmake_minimum_required(VERSION 3.20)
project(lab3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to a Debug build so a bare `cmake -B build` still gives you symbols.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra)

add_executable(calclog   calclog.cpp)
add_executable(sheep     sheep.cpp)
add_executable(fibonacci fibonacci.cpp)
add_executable(tichu     tichu.cpp)
