#include <iostream>

int main() {
  unsigned int lock_1 = 0;
  unsigned int lock_2 = 0;

  std::cin >> lock_1;
  std::cin >> lock_2;

  bool is_unbreakable = ((lock_1 & 0x1) == 0x1) && ((lock_2 & 0x1) == 0x0);

  std::cout << (is_unbreakable ? "no" : "yes");

  return 0;
}