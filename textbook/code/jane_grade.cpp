// jane_grade.cpp -- EC327 Summer 2026
//
// A small synthesis program: std::string, char, uniform initialization,
// and a call to a user-defined function, all in one place.

#include <iostream>
#include <string>

// A function that takes no arguments and returns a double.
double pi() {
  return 3.14;
}

int main() {
  std::string student_name{"Jane Doe"};
  char let{'A'};

  double student_grade{pi()};

  std::cout << "Student: " << student_name << ' ';
  std::cout << student_grade << '\n';
  std::cout << "Grade: " << let << '\n';

  return 0;
}
