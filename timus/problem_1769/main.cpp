#include <iostream>
#include <algorithm>
#include <string>
#include <array>
#include <charconv>

//#define TEST

#ifdef TEST
#include <sstream>
#endif // #ifdef TEST

int main() {
#ifdef TEST
  //std::istringstream in("10123456789\n");
  std::istringstream in("9\n");
  //std::istringstream in("101298765432101\n");
  std::cin.rdbuf(in.rdbuf());
#endif
  std::string value;
  std::cin >> value;

  static constexpr int kDigits = 6;
  static constexpr int kMaxValue = 1000000 - 1;
  static constexpr int kArraySize = kMaxValue / 8 + 1;

  std::array<uint8_t, kArraySize> numbers = { 0 };

  int res = -1;
  int max = -1;

  for (int i = 1; i <= kDigits; ++i) {
    if (value.size() < i) {
      continue;
    }

    for (int j = 0; j < value.size() - i + 1; ++j) {
      if (i > 1 && value[j] == '0') {
        continue;
      }

      int num;
      std::from_chars(value.data() + j, value.data() + j + i, num);
      if (max == -1 || max < num) {
        max = num;
      }

      int idx = num >> 3;
      int sub_idx = 1 << (num & 0x7);
      numbers[idx] |= sub_idx;
    }
  }

  if (max == -1) {
    res = 0;
  } else {
    for (int i = 0; i < numbers.size(); ++i) {
      for (int j = 0; j < 8 * sizeof(uint8_t); ++j) {
        if (i == 0 && j == 0) {
          continue;
        }
        int val = (i << 3) + j;
        if (val > max) {
          res = val;
          break;
        }

        if ((numbers[i] & (1 << j)) == 0x0) {
          res = val;
          break;
        }
      }

      if (res != -1) {
        break;
      }
    }
  }

  if (res == -1) {
    res = max + 1;
  }

  std::cout << res;

  return 0;
}