#include <iostream>
#include <algorithm>
#include <string>
#include <array>
#include <charconv>
#include <vector>

//#define TEST

#ifdef TEST
#include <sstream>
#endif // #ifdef TEST

int main() {
#ifdef TEST
  //std::istringstream in("5\n11011\n11011\n10001\n11111\n11111"); // No
  //std::istringstream in("5\n10001\n10001\n10001\n11111\n11111"); // Yes
  //std::istringstream in("5\n00000\n01110\n01110\n01110\n00000"); // No
  std::istringstream in("7\n0000000\n0000000\n0011111\n0011111\n0011111\n0011111\n0000111"); // Yes
  //std::istringstream in("5\n00111\n01111\n01111\n00000\n00000"); // Yes
  //std::istringstream in("5\n11000\n10000\n10000\n10000\n11000"); // Yes
  //std::istringstream in("5\n00011\n00001\n00001\n00011\n00111"); // Yes
  //std::istringstream in("5\n00000\n01111\n01111\n01110\n00000"); // No

  std::cin.rdbuf(in.rdbuf());
#endif
  int size;
  std::cin >> size;
  std::vector<bool> square(size * size);

  std::string row;
  for (int i = 0; i < size; ++i) {
    std::cin >> row;
    int start_idx = size * i;
    for (int j = 0; j < size; ++j) {
      square[start_idx + j] = row[j] - '0' == 0;
    }
  }

  bool wrong = false;

  // vertical scan
  for (int i = 0; i < size; ++i) {
    // Top to bottom
    int idx_0_to_1 = -1;
    int idx_1_to_0 = -1;
    int ii = -1, jj = -1;
    for (int j = 1; j < size; ++j) {
      int cur_idx = size * j;
      int prev_idx = size * (j - 1);
      auto prev_value = square[prev_idx + i];
      auto cur_value = square[cur_idx + i];

      if (prev_value == cur_value) {
        continue;
      }

      if (prev_value == false) {
        if (idx_0_to_1 == -1) {
          jj = j;
          ii = i;
          idx_0_to_1 = j - 1;
        } else {
          wrong = true;
          break;
        }
      } else {
        if (idx_1_to_0 == -1) {
          idx_1_to_0 = j;
        } else {
          wrong = true;
          break;
        }
      }
    }

    //if (idx_1_to_0 >= 0 && idx_0_to_1 >= 0 && idx_1_to_0 <= idx_0_to_1) {
      if (idx_1_to_0 >= 0 && idx_0_to_1 >= 0) {
      wrong = true;
    }

    if (wrong) {
      break;
    }
  }

  if (!wrong) {
    std::cout << "Yes";
    return 0;
  }

  wrong = false;
  // Horizontal scan
  for (int i = 0; i < size; ++i) {
    int cur_idx = size * i;
    // Right to left
    int idx_0_to_1 = -1;
    int idx_1_to_0 = -1;
    for (int j = 1; j < size; ++j) {
      auto prev_value = square[cur_idx + j - 1];
      auto cur_value = square[cur_idx + j];

      if (prev_value == cur_value) {
        continue;
      }

      if (prev_value == false) {
        if (idx_0_to_1 == -1) {
          idx_0_to_1 = j - 1;
        } else {
          wrong = true;
          break;
        }
      } else {
        if (idx_1_to_0 == -1) {
          idx_1_to_0 = j;
        } else {
          wrong = true;
          break;
        }
      }
    }

    //if (idx_1_to_0 >= 0 && idx_0_to_1 >= 0 && idx_1_to_0 <= idx_0_to_1) {
    if (idx_1_to_0 >= 0 && idx_0_to_1 >= 0) {
      wrong = true;
    }

    if (wrong) {
      break;
    }
  }

  std::cout << (wrong ? "No" : "Yes");
  
  return 0;
}