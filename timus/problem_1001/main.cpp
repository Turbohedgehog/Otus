#include <iostream>
#include <vector>
#include <sstream>
#include <math.h>

int main() {
  unsigned long long num = 0;
  std::vector<double> resuts;
  std::stringstream ss;
  std::string line;

  while (std::getline(std::cin, line)) {
    ss.clear();
    ss.str(line);
    while (ss >> num) {
      double v = static_cast<double>(num);
      resuts.push_back(pow(v, 0.5));
    }
  }

  std::cout.precision(4);
  std::cout << std::fixed;

  for (auto it = resuts.rbegin(); it != resuts.rend(); ++it) {
    std::cout << *it << '\n';
  }

  return 0;
}