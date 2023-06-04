#include <iostream>
#include <sstream>

int main() {
  int k = 0;
  int n = 0;
  std::cin >> k >> n;

  int value;
  int sum = 0;
  for (int i = 0; i < n; ++i) {
    std::cin >> value;
    sum += value - k;
    sum = std::max(0, sum);
  }

  std::cout << sum;

  return 0;
}