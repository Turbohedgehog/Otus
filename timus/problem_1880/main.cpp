#include <iostream>
#include <unordered_set>



int main() {
  unsigned int num_count = 0;
  std::unordered_set<int> owned_numbers_1;
  std::unordered_set<int> owned_numbers_2;
  int number = 0;
  std::size_t res = 0;
  for (int i = 0; i < 3; ++i) {
    std::cin >> num_count;
    auto& owned_numbers = i % 2 == 0 ? owned_numbers_1 : owned_numbers_2;
    auto& owned_numbers_next = i % 2 == 0 ? owned_numbers_2 : owned_numbers_1;
    owned_numbers_next.clear();
    for (unsigned int j = 0; j < num_count; ++j) {
      std::cin >> number;
      if (i == 0) {
        owned_numbers_next.emplace(number);
      } else if (owned_numbers.count(number) > 0) {
        owned_numbers_next.emplace(number);
      }
    }

    res = owned_numbers_next.size();
  }

  std::cout << res;

  return 0;
}