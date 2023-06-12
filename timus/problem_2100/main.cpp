#include <iostream>

int main() {
  unsigned int count = 0;
  std::cin >> count;
  
  std::string guest;
  unsigned int guest_count = 0;
  for (unsigned int i = 0; i < count; ++i) {
    guest.clear();
    ++guest_count;
    std::cin >> guest;
    auto pos = guest.rfind("+one");
    if (pos == std::string::npos) {
      continue;
    }

    if (pos == (guest.length() - 4)) {
      ++guest_count;
    }
  }

  unsigned int total_count = guest_count + 2;

  if (total_count == 13) {
    ++total_count;
  }

  unsigned int total_bill = total_count * 100;
  std::cout << total_bill;

  return 0;
}