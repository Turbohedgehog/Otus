#include <stdio.h>
#include <map>
#include <numeric>

unsigned int segment_tree[262144] = {0};
std::map<unsigned int, std::pair<int, int>> index_map;

int update_gcd(int index) {
  int index_shift = 0;
  int gcd = 1;
  for (int i = 0; i < 17; ++i) {
    int idx1 = index_shift + (index & ~0x1);
    int idx2 = idx1 + 1;
    unsigned int n1 = segment_tree[idx1];
    unsigned int n2 = segment_tree[idx2];
    if (n1 == 1 || n2 == 1) {
      gcd = 1;
    } else if (n1 == 0){
      gcd = n2;
    } else if (n2 == 0) {
      gcd = n1;
    } else {
      gcd = std::gcd(n1, n2);
    }

    index_shift |= (1 << (17 - i));
    index >>= 1;
    segment_tree[index_shift + index] = gcd;
  }
  
  return gcd == 0 ? 1 : gcd;
}

int main() {
  unsigned int input_count = 0;
  scanf("%d", &input_count);
  int last_gcd = 1;
  int index = 0;
  char operation[2];
  unsigned int value;

  for (unsigned int i = 0; i < input_count; ++i) {
    scanf("%s%u", operation, &value);
    auto it = index_map.find(value);

    if (operation[0] == '+') {
      if (it != index_map.end()) {
        if (it->second.first == 0) {
          auto idx = it->second.second;
          segment_tree[idx] = value;
          last_gcd = update_gcd(idx);
        }
        ++it->second.first;
      } else {
        index_map[value] = std::make_pair(1, index);
        segment_tree[index] = value;
        last_gcd = update_gcd(index);
        ++index;
      }
    } else {
      if (it != index_map.end()) {
        if (--it->second.first == 0) {
          auto idx = it->second.second;
          segment_tree[idx] = 0;
          last_gcd = update_gcd(idx);
        }
      }
    }

    printf("%d\n", last_gcd);
  }
  return 0;
}