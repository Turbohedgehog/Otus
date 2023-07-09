//#include <iostream>
#include <algorithm>
#include <stdio.h>

//#define TEST

#ifdef TEST
#include <sstream>
#endif // #ifdef TEST

int main() {
#ifdef TEST
  std::istringstream in("2 3\n4\n8\n2\n7\n3\n");
  //std::istringstream in("4 3\n12\n4\n12\n3\n21\n5\n16\n");
  std::cin.rdbuf(in.rdbuf());
#endif
  std::size_t n, m;
  scanf("%zu %zu", &n, &m);
  //std::cin >> n >> m;

  unsigned int value;
  unsigned int first_vertical = 0;
  unsigned int last_vertical = 0;
  unsigned int max_vertical = 0;
  unsigned int first_horizontal = 0;
  unsigned int last_horizontal = 0;
  unsigned int max_horizontal = 0;

  for (std::size_t i = 0; i < n; ++i) {
    //std::cin >> value;
    scanf("%u", &value);
    if (i == 0) {
      first_vertical = value;
    } else if (i == n - 1) {
      last_vertical = value;
    } else {
      if (max_vertical == 0 || max_vertical < value) {
        max_vertical = value;
      }
    }
  }

  for (std::size_t i = 0; i < m; ++i) {
    //std::cin >> value;
    scanf("%u", &value);

    if (i == 0) {
      first_horizontal = value;
    } else if (i == m - 1) {
      last_horizontal = value;
    } else {
      if (max_horizontal == 0 || max_horizontal < value) {
        max_horizontal = value;
      }
    }
  }

  unsigned int way1 = std::min(first_vertical, last_horizontal);
  unsigned int way2 = std::min(first_horizontal, last_vertical);
  unsigned int way3 = max_vertical > 0 ? std::min(std::min(first_horizontal, last_horizontal), max_vertical) : way1;
  unsigned int way4 = max_horizontal > 0 ? std::min(std::min(first_vertical, last_vertical), max_horizontal) : way2;
  
  unsigned int best = std::max(std::max(way1, way2), std::max(way3, way4));

  printf("%u", best);
  //std::cout << best;

  return 0;
}