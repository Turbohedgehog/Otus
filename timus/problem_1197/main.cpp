#include <iostream>
#include <vector>

bool is_cell_in_the_board(int x, int y) {
  return x >= 0 && x <= 7 && y >= 0 && y <= 7;
}

unsigned int calc_attacked_cell_count(const std::string& cell) {
  int x = cell[0] - 'a';
  int y = atoi(&cell[1]) - 1;

  unsigned int count = 0;

  for (int i = -2; i < 3; i += 4) {
    for (int j = -1; j < 2; j += 2) {
      if (is_cell_in_the_board(x + i, y + j)) {
        ++count;
      }
      if (is_cell_in_the_board(x + j, y + i)) {
        ++count;
      }
    }
  }

  return count;
}

int main() {
  unsigned int test_count = 0;
  std::vector<unsigned int> results;
  std::cin >> test_count;
  std::string cell;
  for (unsigned int i = 0; i < test_count; ++i) {
    std::cin >> cell;
    results.push_back(calc_attacked_cell_count(cell));
  }

  for (const auto& res : results) {
    std::cout << res << '\n';
  }

  return 0;
}