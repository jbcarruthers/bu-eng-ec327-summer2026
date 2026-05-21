// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: L3 snippet -- a 2D grid, three ways: vector-of-vectors,
//          array-of-arrays, and a flat row-major block.
// Topic: 2d-structures
// Category: demo
// Lecture: L3
//
// The grid: 12 months x 14 possible pay schedules (the first payday
// can land on any of days 1..14). Here we just store one cell value
// each way -- the point is the three storage shapes, not the puzzle.
//
// Build & run:
//   g++ -std=c++20 -Wall l3_grid2d.cpp -o l3_grid2d
//   ./l3_grid2d

#include <array>
#include <iostream>
#include <vector>

int main() {
  const int months = 12;
  const int schedules = 14;

  // (1) VECTOR OF VECTORS -- dynamic; each row separately allocated.
  std::vector<std::vector<int>> grid_a(
      months, std::vector<int>(schedules, 0));
  grid_a[2][5] = 3;                             // March, schedule 5
  std::cout << "vec-of-vec    cell(2,5) = " << grid_a[2][5] << '\n';

  // (2) ARRAY OF ARRAYS -- size 12x14 fixed at compile time.
  std::array<std::array<int, 14>, 12> grid_b = {};
  grid_b[2][5] = 3;
  std::cout << "arr-of-arr    cell(2,5) = " << grid_b[2][5] << '\n';

  // (3) FLAT, ROW-MAJOR -- one contiguous block; index by hand.
  //     Cell (row, col) lives at row * width + col.
  std::vector<int> grid_c(months * schedules, 0);
  grid_c[2 * schedules + 5] = 3;
  std::cout << "flat row-maj  cell(2,5) = "
            << grid_c[2 * schedules + 5] << '\n';

  return 0;
}
