#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>

//#define TEST

#ifdef TEST
#include <sstream>
#endif // #ifdef TEST


std::tuple<bool, bool, double> check_point(int x, int y, int px0, int py0, int px1, int py1) {
  int diff_x = x - px0;
  int diff_y = y - py0;
  int dir_x = px1 - px0;
  int dir_y = py1 - py0;

  bool is_inside = (dir_x * diff_y - dir_y * diff_x >= 0.);
  int delta_x = -diff_x;
  int delta_y = -diff_y;

  int dir_dot = dir_x * dir_x + dir_y * dir_y;
  if (dir_dot == 0) {
    return std::make_tuple(is_inside, false, 0.);
  }

  int delta_dot = dir_x * delta_x + dir_y * delta_y;
  double t = -static_cast<double>(delta_dot) / static_cast<double>(dir_dot);
  t = std::clamp(t, 0., 1.);

  double px = static_cast<double>(px0) + t * static_cast<double>(dir_x);
  double py = static_cast<double>(py0) + t * static_cast<double>(dir_y);

  double diff_px = px - x;
  double diff_py = py - y;

  double distance_sqr = diff_px * diff_px + diff_py * diff_py;
  return std::make_tuple(is_inside, true, distance_sqr);
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
#ifdef TEST
  //std::istringstream in("2 -1 8\n0 1\n1 0\n2 0\n3 1\n3 2\n2 3\n1 3\n0 2"); // -> 2.0
  //std::istringstream in("3 2 8\n0 1\n1 0\n2 0\n3 1\n3 2\n2 3\n1 3\n0 2"); // -> 0.0
  //std::istringstream in("0 0 3\n100 1\n0 2\n-100 1"); // -> 2.0
  //std::istringstream in("2 1 8\n0 1\n1 0\n2 0\n3 1\n3 2\n2 3\n1 3\n0 2"); // -> 0.0
  //std::istringstream in("0 0 3\n1 0\n3 0\n2 3"); // -> 2.0
  //std::istringstream in("3 0 8\n0 2\n2 0\n4 0\n6 2\n6 4\n4 6\n2 6\n0 4"); // -> 0.0
  //std::istringstream in("0 0 8\n0 2\n2 0\n4 0\n6 2\n6 4\n4 6\n2 6\n0 4"); // -> 2.828
  //std::istringstream in("0 0 4\n1 1\n-1 1\n-1 -1\n1 -1"); // -> 0.0
  //std::istringstream in("-1000000 -1000000 3\n999999 1000000\n1000000 999999\n1000000 1000000"); // -> 5656852.835
  //std::istringstream in("2 0 3\n4 0\n3 3\n0 3"); // -> 2.4
  //std::istringstream in("-1000000 -1000000 3\n0 1000000\n-1000000 1000000\n1000000 999531"); // -> 3999999.890
  std::istringstream in("-1000000 1000000 4\n0 -1000\n600 0\n0 1000\n-1000 0"); // -> 2827012.911
  std::cin.rdbuf(in.rdbuf());
#endif

  int x, y;
  int n;

  std::cin >> x >> y >> n;
  bool is_inside = true;

  int first_px, first_py;
  int prev_px, prev_py;
  double min_sqr = 0.;
  int diff_x, diff_y;
  int px, py;
  for (unsigned int i = 0; i < n; ++i) {
    std::cin >> px >> py;
    
    if (i == 0) {
      diff_x = px - x;
      diff_y = py - y;
      min_sqr = static_cast<double>(diff_x * diff_x + diff_y * diff_y);
      first_px = px;
      first_py = py;
      prev_px = px;
      prev_py = py;

      continue;
    }

    auto res = check_point(x, y, prev_px, prev_py, px, py);
    is_inside = is_inside && std::get<0>(res);
    if (std::get<1>(res)) {
      min_sqr = std::min(min_sqr, std::get<2>(res));
    }

    prev_px = px;
    prev_py = py;
  }

  auto res = check_point(x, y, px, py, first_px, first_py);
  is_inside = is_inside && std::get<0>(res);
  if (std::get<1>(res)) {
    min_sqr = std::min(min_sqr, std::get<2>(res));
  }

  double d = 0.;

  if (!is_inside) {
    d = std::sqrt(min_sqr);
    d *= 2.0;
  }

  std::cout << std::fixed << std::setprecision(3) << d;

  return 0;
}