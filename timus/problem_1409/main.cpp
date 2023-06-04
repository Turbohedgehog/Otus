#include <iostream>

int main() {
  unsigned int harry_counts = 0;
  unsigned int larry_counts = 0;

  std::cin >> harry_counts >> larry_counts;

  unsigned int sum = harry_counts + larry_counts - 1;
  unsigned int harry_missed = sum - harry_counts;
  unsigned int larry_missed = sum - larry_counts;

  std::cout << harry_missed << " " << larry_missed;

  return 0;
}