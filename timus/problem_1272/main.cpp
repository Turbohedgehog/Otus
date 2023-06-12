#include <iostream>
#include <vector>
//#include <sstream>

struct Island {
  std::vector<unsigned int> tonnel_neighbors;
  unsigned int tonnel_group = 0;
};

struct TonnelGroup {
  std::vector<unsigned int> bridge_neighbors;
  unsigned int bridge_group = 0;
};

using IslandContainer = std::vector<Island>;
using TonnelGroupContainer = std::vector<TonnelGroup>;

std::vector<unsigned int> neighbors_1;
std::vector<unsigned int> neighbors_2;

unsigned int mark_tonnel_groups(IslandContainer& islands) {
  unsigned int tonnel_group = 0;
  for (unsigned int i = 0; i < islands.size(); ++i) {
    auto& island = islands[i];
    if (island.tonnel_group != 0) {
      continue;
    }

    ++tonnel_group;
    island.tonnel_group = tonnel_group;

    neighbors_1.clear();
    neighbors_2.clear();
    auto* src = &neighbors_1;
    auto* dst = &neighbors_2;
    src->push_back(i);
    while (!src->empty()) {
      dst->clear();
      for (std::size_t src_idx = 0; src_idx < src->size(); ++src_idx) {
        auto island_idx = src->at(src_idx);
        auto& tonnel_neighbors = islands[island_idx].tonnel_neighbors;
        for (auto& tn_idx : tonnel_neighbors) {
          auto& tonnel_neighbor = islands[tn_idx];
          if (tonnel_neighbor.tonnel_group == 0) {
            tonnel_neighbor.tonnel_group = tonnel_group;
            dst->push_back(tn_idx);
          }
        }
      }
      std::swap(src, dst);
    }
  }

  return tonnel_group;
}

int main() {
  unsigned int islands_count = 0;
  unsigned int bridges_count = 0;
  unsigned int tonnels_count = 0;

  //std::istringstream input("6 3 4\n1 2\n2 3\n4 5\n1 3\n3 4\n4 6\n5 6");
  //std::cin.rdbuf(input.rdbuf());

  std::cin >> islands_count >> tonnels_count >> bridges_count;

  std::vector<Island> islands(islands_count);

  for (unsigned int i = 0; i < tonnels_count; ++i) {
    unsigned int id1, id2;
    std::cin >> id1 >> id2;
    islands[id1 - 1].tonnel_neighbors.push_back(id2 - 1);
    islands[id2 - 1].tonnel_neighbors.push_back(id1 - 1);
  }

  auto tonnel_groups = mark_tonnel_groups(islands);

  std::cout << tonnel_groups - 1;

  return 0;
}