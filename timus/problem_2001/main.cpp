#include <iostream>

int main() {
  constexpr int kCount = 3;
  unsigned int a[kCount] = {0};
  unsigned int b[kCount] = {0};

  for (int i = 0; i < kCount; ++i) {
    std::cin >> a[i] >> b[i];
  }

  unsigned int bucket_weights[2] = {a[2], b[1]};
  unsigned int berry_weights[2] = {a[0] - bucket_weights[0], b[0] - bucket_weights[1]};

  std::cout << berry_weights[0] << " " << berry_weights[1];

  return 0;
}