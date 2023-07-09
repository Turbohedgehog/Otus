#include <iostream>
#include <algorithm>
#include <vector>
#include <algorithm>

//#define TEST

#ifdef TEST
#include <sstream>
#endif // #ifdef TEST

int main() {
#ifdef TEST
  //std::istringstream in("3\n170\n170\n180\n175\n170\n175\n175\n170\n175\n");
  std::istringstream in("5\n170\n170\n180\n175\n170\n175\n175\n170\n175\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n");
  std::cin.rdbuf(in.rdbuf());
#endif
  int n, n_sqr;
  std::cin >> n;
  n_sqr = n * n;
  std::vector<uint8_t> data(n_sqr);
  std::vector<uint8_t> res(n_sqr);

  uint16_t value;
  for (int i = 0; i < n_sqr; ++i) {
    std::cin >> value;
    data[i] = static_cast<uint8_t>(value);
  }

#ifdef TEST
  for (int i = 0; i < n_sqr; ++i) {
    std::cout << +data[i] << '\n';
  }

  std::cout << "================================\n";
#endif

  std::sort(data.rbegin(), data.rend());

#ifdef TEST
  for (int i = 0; i < n_sqr; ++i) {
    std::cout << +data[i] << '\n';
  }

  std::cout << "================================\n";
#endif

  int center = n >> 1;
  int square_count = n - center;
  int max_square_size_width = square_count;

#ifdef TEST
  std::cout << "center = " << center << '\n';
  std::cout << "square_count = " << square_count << '\n';
  std::cout << "max_square_size_width = " << max_square_size_width << '\n';
#endif

  for (int y = 0; y < n; ++y) {
    for (int x = 0; x < n; ++x) {
      if (x == center && y == center) {
        std::cout << +data[0] << ' ';
        //std::cout << +0 << ' ';
        continue;
      }

      int square_id = std::max(std::abs(center - x), std::abs(center - y));
      int square_shift = square_count - square_id - 1;
      int square_size_width = (square_id << 1) + 1;
      int local_x = x - square_shift;
      int local_y = y - square_shift;
      //int start_id = ((square_id - 1) << 2) + 1;
      int start_id = square_size_width - 2;
      start_id *= start_id;
      int side_id;
      int order;
      if (local_y == 0 && local_x > 0) {
        side_id = 0; // up
        int xc = x - center;
        if (xc > 0) {
          order = (xc << 1) - 1;
        } else {
          order = (-xc) << 1;
        }
      } else if (local_x == square_size_width - 1 && local_y > 0) {
        side_id = 1; // right
        int yc = y - center;
        if (yc > 0) {
          order = (yc << 1) - 1;
        } else {
          order = (-yc) << 1;
        }
      } else if (local_y == square_size_width - 1 && local_x < square_size_width - 1) {
        side_id = 2; // down
        int xc = center - x;
        if (xc > 0) {
          order = (xc << 1) - 1;
        } else {
          order = (-xc) << 1;
        }
      } else {
        side_id = 3; // left
        int yc = center - y;
        if (yc > 0) {
          order = (yc << 1) - 1;
        } else {
          order = (-yc) << 1;
        }
      }

      order <<= 2;

      int id = start_id + side_id + order;
      std::cout << +data[id] << ' ';  
      //std::cout << +id << ' ';
    }

    std::cout << '\n';
  }

  return 0;
}