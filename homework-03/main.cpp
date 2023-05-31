#include "my_allocator.hpp"
#include "my_container.hpp"

#include <map>
#include <iostream>

int main (int, char **) {
  using my_allocator = homework_03::MyAllocator<std::pair<const int, int>, 10, true, true>;

  // Just map
  std::map<int, int> factorial_map;
  factorial_map[0] = 1;
  for (int i = 1; i < 10; ++i) {
    factorial_map[i] = factorial_map[i - 1] * i;
  }

  for (const auto& f_pair : factorial_map) {
    std::cout << f_pair.first << " " << f_pair.second << "\n";
  }

  // Map with my allocator
  std::map<int, int, std::less<>, my_allocator> factorial_map_ext;
  factorial_map_ext[0] = 1;
  for (int i = 1; i < 10; ++i) {
    factorial_map_ext[i] = factorial_map_ext[i - 1] * i;
  }

  for (const auto& f_pair : factorial_map_ext) {
    std::cout << f_pair.first << " " << f_pair.second << "\n";
  }

  // My container with default allocator
  homework_03::MyContainer<int> my_container;
  for (int i = 0; i < 10; ++i) {
    my_container.push_back(i);
  }

  // My container with custom allocator
  homework_03::MyContainer<int, my_allocator> my_container_ext;
  for (int i = 0; i < 10; ++i) {
    my_container.push_back(i);
  }

  for (const auto& v : my_container) {
    std::cout << v << "\n";
  }

  for (const auto& v : my_container_ext) {
    std::cout << v << "\n";
  }

  return 0;
}