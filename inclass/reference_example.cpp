#include <iostream>
using std::cout;

int tester(int n, int& y, const int& z, int* pw) {
  cout << &n << "\n";
  cout << &y << "\n";
  cout << &z << "\n";
  cout << pw << "\n";
  cout << &pw << "\n\n";
  return 0;
}

int main() {
  int a, b, c;
  a = 456;
  b = 123;
  c = 789;
  cout << &a << "\n";
  cout << &b << "\n";
  cout << &c << "\n\n";

  int d;
  d = tester(a, b, c, &a);
}