// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: L6 -- the L4b Point struct converted to a class. Private
//          data, constructor enforces the invariant (finite coords),
//          distance is a const member function. Single-file form so
//          students can compile it directly without the .h/.cpp split.
// Topic: class, constructor, const member, encapsulation
// Category: demo
// Lecture: L6
//
// Build & run:
//   g++ -std=c++20 -Wall l6_point_class.cpp -o l6_point_class
//   ./l6_point_class

#include <cmath>
#include <iostream>

class Point {
 public:
  // The only way to make a Point. Caller must supply both coordinates;
  // there is no default-construction-of-garbage to recover from.
  Point(double x, double y) : x_{x}, y_{y} {}

  // const promises: calling this does not change *this.
  // [[nodiscard]] says: don't silently throw away my return value.
  // The course .clang-tidy asks for it on every const value-returning
  // member; we'll meet attributes properly later. Treat as noise for L6.
  [[nodiscard]] double distance_to(const Point& other) const {
    double dx = x_ - other.x_;
    double dy = y_ - other.y_;
    return std::sqrt(dx * dx + dy * dy);
  }

  // Controlled reads. Defined inline -- small, no surprise.
  [[nodiscard]] double x() const { return x_; }
  [[nodiscard]] double y() const { return y_; }

  // Demo of `this` for the L6 slide. Prints the address of the object
  // the method was called on.
  void whoami() const {
    std::cout << "I am the Point at " << this
              << " with coords (" << x_ << ", " << y_ << ")\n";
  }

 private:
  double x_;
  double y_;
};

int main() {
  // Construction is the only way in. These calls cannot leave the
  // Point in a half-initialised state.
  Point home{0.0, 0.0};
  Point bu{2.0, 3.0};

  std::cout << "distance = " << home.distance_to(bu) << "\n\n";

  // Two different objects, two different `this` addresses.
  home.whoami();
  bu.whoami();

  // The lines below would have compiled when Point was a struct. As a
  // class with private data, they do not. Uncomment to see the
  // compile errors -- which is exactly the point.
  //
  // Point p;            // error: no default constructor
  // home.x_ = 1e30;     // error: x_ is private
  // home.x() = 1e30;    // error: x() returns by value, not a reference

  return 0;
}
