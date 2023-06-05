#include <iostream>


int main() {
  int a, b, c;
  std::cin >> a;
  std::cin >> b;
  std::cin >> c;

  int res = std::min(a - b - c, a - b * c);
  std::cout << res;

  return 0;
}