#include <iostream>
#include <cmath>

#define TEST

#ifdef TEST
#include <sstream>
#endif // #ifdef TEST


int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
#ifdef TEST
  //std::istringstream in("28"); // -> 7
  //std::istringstream in("50005000");
  std::istringstream in("48805971417498085602366608859366"); // -> 9879875648761788
  std::cin.rdbuf(in.rdbuf());
#endif

  uint64_t num;
  std::cin >> num;
  uint64_t d = static_cast<uint64_t>(std::sqrt(static_cast<long double>(1 + (num << 3))));
  uint64_t n = (d - 1) >> 1;
  std::cout << n;

  return 0;
}