// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: Overloading operator<< and operator>> so YOUR class flows through
//          streams -- the extensibility that printf can't match.
// Topic: io-operator-overload
// Category: demo
// Lecture: L13

// L13, part 5 -- the payoff of the stream design. << and >> are just
// operators, so you can OVERLOAD them for your own types. Teach C++ how to
// print a Point ONCE, and that one function works on EVERY stream: std::cout,
// std::cerr, a file (ofstream), an in-memory string (ostringstream) -- all of
// them, for free.
//
// This is the honest answer to "why keep streams around if std::format is the
// default?":
//   printf      -- CANNOT be extended. There is no %-code for your Point, and
//                  no way to add one. You're stuck pulling fields out by hand.
//   operator<<  -- ONE short free function and your type streams everywhere.
//                  This is the idiomatic C++ way to make a type printable.
//   std::format -- CAN be extended too, but it costs more: a std::formatter
//                  specialization (shown at the bottom for contrast).
//
// We reuse the L5/L6 Point: private coordinates, public accessors. Because the
// accessors are public, operator<< needs NO `friend` -- it reads through x()
// and y() like any other caller. Reach for `friend` only when an operator
// truly needs the private members.
//
// Build:  g++ -std=c++20 -Wall -Wextra -O0 -g l13_streamops.cpp -o l13_streamops
// Run:    ./l13_streamops

#include <format>
#include <iostream>
#include <sstream>  // std::istringstream, std::ostringstream
#include <string>

namespace {

class Point {
 public:
  Point() = default;  // a default Point (0,0) so operator>> can fill one in
  Point(double x, double y) : x_{x}, y_{y} {}

  [[nodiscard]] double x() const { return x_; }
  [[nodiscard]] double y() const { return y_; }

  void set(double x, double y) {
    x_ = x;
    y_ = y;
  }

 private:
  double x_ = 0.0;
  double y_ = 0.0;
};

// operator<< -- a FREE function (not a member): the left operand is the stream,
// not the Point, so it can't be a Point method. Take the stream by reference,
// the Point by const reference, and RETURN the stream so calls CHAIN:
//   std::cout << a << " -> " << b << "\n";
// Note std::ostream, not std::cout -- so this also works on a file or a
// stringstream. That generality is the whole point.
std::ostream& operator<<(std::ostream& os, const Point& p) {
  return os << std::format("({}, {})", p.x(), p.y());
}

// operator>> -- the input twin. Read two doubles; if either read fails, the
// stream's fail bit is already set and the caller's `while (in >> p)` stops.
// We only write into `p` after both reads succeed.
std::istream& operator>>(std::istream& is, Point& p) {
  double x = 0.0;
  double y = 0.0;
  if (is >> x >> y) {
    p.set(x, y);
  }
  return is;
}

}  // namespace

// std::formatter<Point> -- the std::format extension point, shown for CONTRAST.
// This is what it costs to make `std::format("{}", p)` work: a specialization
// with parse() + format(). Compare to the single one-line operator<< above.
// (This minimal version ignores any format spec inside the braces.) Must live
// at namespace scope, not inside the anonymous namespace.
template <>
struct std::formatter<Point> {
  static constexpr auto parse(std::format_parse_context& ctx) {
    return ctx.begin();
  }
  static auto format(const Point& p, std::format_context& ctx) {
    return std::format_to(ctx.out(), "({}, {})", p.x(), p.y());
  }
};

int main() {
  const Point home{0.0, 0.0};
  const Point bu{2.0, 3.0};

  // (1) One operator<<, and Point flows into std::cout -- and it CHAINS.
  std::cout << "to cout:   " << home << " -> " << bu << "\n";

  // (2) The SAME operator<< works on an in-memory string stream (ostringstream)
  //     -- "format a Point into a std::string" with zero extra code.
  std::ostringstream oss;
  oss << bu;
  const std::string s = oss.str();
  std::cout << "to string: captured \"" << s << "\"\n";

  // (3) ... and on a file stream, and back. Round-trip a Point through text:
  //     write it with <<, read it back with >>.
  std::ostringstream file_like;  // stand-in for an ofstream, kept in memory
  file_like << "5 12\n";         // the text a file would contain: "5 12"
  std::istringstream in(file_like.str());
  Point read_back;
  in >> read_back;  // operator>> parses the two numbers into the Point
  std::cout << "from text: read back " << read_back << "\n";

  // (4) Because operator<< / >> are the SAME interface as cin/cout, a
  //     `while (in >> p)` loop reads a whole file of Points unchanged (L13
  //     part 3). Here we read three from one string:
  std::istringstream stream("1 1  2 4  3 9");
  std::cout << "loop:     ";
  Point p;
  while (stream >> p) {
    std::cout << p << " ";
  }
  std::cout << "\n";

  // (5) The contrast: std::format also prints our Point now -- but only because
  //     we wrote the std::formatter<Point> specialization above. operator<<
  //     was one line; this was a whole struct.
  std::cout << std::format("to format: {} (needed a std::formatter<Point>)\n",
                           bu);
  return 0;
}
