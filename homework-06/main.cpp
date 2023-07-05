
#include <iostream>

#include "matrix.hpp"

int main (int, char **) {
  homework_06::Matrix<int, 0> matrix;
  for (int i = 0; i < 10; ++i) {
    matrix[i][i] = i;
  }

  for (int i = 9; i >= 0; --i) {
    matrix[9 - i][i] = i;
  }

  for (int i = 1; i <= 8; ++i) {
    for (int j = 1; j <= 8; ++j) {
      auto t = matrix[i][j];
      std::cout << t << " ";
    }
    std::cout << "\n";
  }

  std::cout << matrix.size() << "\n";

  for (auto c : matrix) {
    int x;
    int y;
    int v;
    std::tie(x, y, v) = c;

    std::cout << "m[" << x << ", " << y << "] = " << v << "\n";
  }

  return 0;
}