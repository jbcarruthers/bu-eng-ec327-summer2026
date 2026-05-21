//
//    Read the day-of-year of the year's first payday; print every
//    payday with its month. EXACTLY the same result as stage 1 --
//    the day->month lookup is just now a function you write and call.
//

// magic numbers included.

#include <atomic>
#include <ios>
#include <iostream>

//
// while :
//   A
//   B
//   C

int main() {
  int current_day = 5;
  int month_length = 30;
  int month_number = 1;
  int day_of_month = current_day;
  int paychecks_this_month = 0;

  while (current_day <= 365) {
    paychecks_this_month += 1;

    if (paychecks_this_month > 2) {
      std::cout << month_number << " " << current_day << "three!\n";
    }

    current_day = current_day + 14;
    day_of_month = day_of_month + 14;

    if (month_number <= 7 and day_of_month > 30) {
      day_of_month = day_of_month - 30;
      month_number += 1;
      paychecks_this_month = 0;
    }
    if (month_number > 7 and day_of_month > 31) {
      day_of_month = day_of_month - 31;
      month_number += 1;
      paychecks_this_month = 0;
    }
  }
  return 0;
}