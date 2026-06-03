// Copyright jbc@bu.edu 2026

#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::vector;

bool is_leap_year(std::string year) {
  if (year == "2000") {
    return true;
  }
  return false;
}

bool is_leap_year(int year) {
  if (year % 4 > 0) {
    return false;
  }
  if (year % 400 == 0) {
    return true;
  }
  // if (year % 100 != 0) {
  // 	 return true;
  // }
  // else {
  // 	 return false;
  // }

  return year % 100 != 0;
}

int fcn(
  int a,          // pass by value (local copy)
  const int b,    // pass by value (local copy), but b is declared const
  int & c,        // pass by reference
  const int & d,  // pass by const reference
  int* e   // pass by pointer
  )
 {
}


void print_calendar( ) {


}

void sort_appointments(first, last, howtosort) {
// change appoinments between first and last based 
// on sort criteria
}


vector<appointments> sorted_appointments(theappoinment) 




int main() {
  vector<int> years = {2026, 2024, 2000, 1900};

  for (auto y : years) {
    cout << y << " " << is_leap_year(y) << "\n";
  }

  add_third_to_first_two(x,y,n);
  int m = total_length(years);

  years.push_back(2005);
  int m = years.front();
  // except: v.pop();

  float i = interest() ;


}