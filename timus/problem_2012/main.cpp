#include <iostream>

int main() {
  constexpr int kTaskCount = 12;
  constexpr int kTotalMinutes = 5 * 60;
  constexpr int kOtherTaskCount = (kTotalMinutes - 60) / 45;
  int f = 0;
  std::cin >> f;

  bool will_resolve = (f + kOtherTaskCount >= kTaskCount);
  std::string answer = will_resolve ? "YES" : "NO";
  std::cout << answer;

  return 0;
}