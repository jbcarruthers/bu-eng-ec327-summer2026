#include "Point.h"
#include <iostream>

int main() {
  Point home{0.0, 0.0};
  Point bu{2.0, 3.0};
  std::cout << distance(home, bu) << "\n";
}
