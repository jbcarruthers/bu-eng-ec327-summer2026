# CMakeLists.txt — Lab 2. Picks up where Lab 1's one-line version left off.
#
# Two new ideas over Lab 1:
#   1. `poem` is now built from TWO sources (poem.cpp + greet.cpp) — CMake
#      compiles each and links them, the same job you did by hand in Part 0.
#   2. `broken` has an optional sanitizer build, toggled from the command line:
#         cmake -B build                          # broken built normally
#         cmake -B build -DENABLE_SANITIZERS=ON   # broken built with ASan+UBSan
cmake_minimum_required(VERSION 3.20)
project(lab2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

# Part 0 — the two-file build. poem.cpp calls greeting(), which lives in
# greet.cpp; greet.h is the declaration both files agree on.
add_executable(poem poem.cpp greet.cpp)

# Part 3 — the sanitizer target. OFF by default so a plain `cmake -B build`
# still gives you a normal binary; flip it on to make broken catch fire.
option(ENABLE_SANITIZERS "Build the broken target with ASan + UBSan" OFF)
add_executable(broken broken.cpp)
if(ENABLE_SANITIZERS)
  target_compile_options(broken PRIVATE -fsanitize=address,undefined -g -O0)
  target_link_options(broken PRIVATE -fsanitize=address,undefined)
endif()
