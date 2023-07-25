//#include <iostream>
#include <iostream>
#include <algorithm>
#include <vector>

//#define TEST

#ifdef TEST
#include <sstream>
#endif // #ifdef TEST

struct City {
  std::vector<std::pair<std::size_t, unsigned int>> neightbors;
  bool marked = false;
};


using CityContainer = std::vector<City>;
// 0 - max strait lenght
// 1 - max subtree lenght
// 2 - found or loop
using CheckRouteRes = std::tuple<unsigned int, unsigned int, bool>;

CheckRouteRes check_route(
    //const RoadContainer& roads,
    CityContainer& cities,
    unsigned long leght,
    std::size_t check_idx,
    const City* city_from = nullptr) {
  auto& check_city = cities[check_idx];
  // Check loop
  if (check_city.marked) {
    return CheckRouteRes(0, 0, true);
  }

  check_city.marked = true;

  unsigned int max = 0;
  unsigned int max1 = 0;
  unsigned int max2 = 0;
  unsigned int max_subtree = 0;

  for (const auto& road : check_city.neightbors) {
    auto& city = cities[road.first];
    // Avoid source city
    if (&city == city_from) {
      continue;
    }

    auto res = check_route(cities, leght, road.first, &check_city);
    if (std::get<bool>(res)) {
      return res;
    }

    max_subtree = std::max(max_subtree, std::get<1>(res));

    auto len = std::get<0>(res) + road.second;
    if (max2 < len) {
      if (max1 < len) {
        max2 = max1;
        max1 = len;
      } else {
        max2 = len;
      }
    }

    max = std::max(max, len);
  }

  auto subtree_len = std::max(max1 + max2, max_subtree);

  return CheckRouteRes(max, subtree_len, max >= leght || subtree_len >= leght);
}

bool check_routes(CityContainer& cities, unsigned long leght) {
  for (std::size_t i = 0; i < cities.size(); ++i) {
    if (cities[i].marked) {
      continue;
    }

    auto res = check_route(cities, leght, i);
    if (std::get<bool>(res)) {
      return true;
    }
  }

  return false;
}

int main() {
#ifdef TEST
  //std::istringstream in("3 2 15\n1 2 10\n2 3 5");
  //std::istringstream in("3 2 20\n1 2 10\n2 3 5");
  //std::istringstream in("3 3 1000\n1 2 1\n2 3 1\n1 3 1");
  std::istringstream in("1 1 5\n1 1 2");
  //std::istringstream in("4 3 12\n1 2 5\n1 3 4\n3 4 4");
  std::cin.rdbuf(in.rdbuf());
#endif

  unsigned int m, n;
  unsigned long s;
  unsigned int p, q, r;

  //std::cin >> m >> n >> s;
  scanf("%u %u %u", &m, &n, &s);

  CityContainer cities(m);

  bool has_allowed_road = false;

  for (unsigned int i = 0; i < n; ++i) {
    //std::cin >> p >> q >> r;
    scanf("%u %u %u", &p, &q, &r);
    if (p == q) {
      has_allowed_road = true;
    }

    if (r >= s) {
      has_allowed_road = true;
    }

    if (has_allowed_road) {
      continue;
    }

    cities[p - 1].neightbors.push_back(std::make_pair(q - 1, r));
    cities[q - 1].neightbors.push_back(std::make_pair(p - 1, r));
  }

  if (has_allowed_road || check_routes(cities, s)) {
    //std::cout << "YES";
    printf("YES");
  } else {
    //std::cout << "NO";
    printf("NO");
  }

  return 0;
}