#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <iomanip>

int main() {
  double a, r;
  std::cin >> a >> r;
  double d = 2. * r;

  double res = 0.;

  if (d <= a) {
    res = M_PI * r * r;
  } else if (d >= sqrt(2. * a * a)) {
    res = a * a;
  } else {
    double a_2 = a / 2.;
    double b_2 = sqrt(r * r - a_2 * a_2);
    double st = 0.5 * a_2 * b_2;
    double alpha = atan2(b_2, a_2);
    double betta = M_PI_4 - alpha;
    double ss = r * r * betta * 0.5;
    res = (st + ss) * 8.;
  }
  
  std::cout << std::fixed << std::setprecision(3) << res;

  return 0;
}