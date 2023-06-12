#include <iostream>
#include <string>

int main() {
  unsigned int monsters = 0;
  std::cin >> monsters;
  std::string localized;
  if (monsters >= 1 && monsters <= 4) {
    localized = "few";
  } else if (monsters <= 9) {
    localized = "several";
  } else if (monsters <= 19) {
    localized = "pack";
  } else if (monsters <= 49) {
    localized = "lots";
  } else if (monsters <= 99) {
    localized = "horde";
  } else if (monsters <= 249) {
    localized = "throng";
  } else if (monsters <= 499) {
    localized = "swarm";
  } else if (monsters <= 999) {
    localized = "zounds";
  } else {
    localized = "legion";
  }

  std::cout << localized;

  return 0;
}