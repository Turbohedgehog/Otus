#include <iostream>

int main() {
  int k = 0;
  int n = 0;
  std::cin >> n >> k;
  if (k < 0) {
    return 0;
  }

  int total_time = 0;

  if (n < 0) {
    std::cout << total_time;
    return 0;
  }
  
  int first_side_cook_attempts = n / k;
  if (n < k) {
    ++first_side_cook_attempts;
  }
  total_time += first_side_cook_attempts;

  int one_side_cooked = first_side_cook_attempts * k;
  int raw_count = n - one_side_cooked;
  
  if (raw_count > 0) {
    ++total_time;
    one_side_cooked += raw_count;
    one_side_cooked -= k - raw_count;
    one_side_cooked = std::max(one_side_cooked, 0);
  }

  if (one_side_cooked > 0) {
    total_time += one_side_cooked / k;
    if (one_side_cooked % k > 0) {
      ++total_time;
    }
  }

  std::cout << total_time;

  return 0;
}