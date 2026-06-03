#include <cmath>
#include <iostream>

class DayOfMonth {
 public:
  DayOfMonth(int date) {
    std::cout << "in constructor\n";
    this->date = date;
  }

  int get_date() {
    std::cout << (*this).date << " or " << this->date << "\n";
    return date;
  }

 private:
  int date;
};
// Start
// in constructor
// in constructor
// Hello 0x16f35ed9c
// 4
// Hello 0x16f35ed98
// 12

int main() {
  std::cout << "Start\n";
  DayOfMonth payday{4};
  DayOfMonth paydaytwo{12};
  std::cout << "Hello " << payday.get_date() << "\n";
  std::cout << "Hello " << paydaytwo.get_date() << "\n";
}