#include <iostream>

#include "ip_filter.h"


int main (int, char **) {
  try {
    homework_02::IPFilter ip_filter;

    for(std::string line; std::getline(std::cin, line);) {
      if (line.empty()) {
        break;
      }

      ip_filter.AddAddress(line);
    }

    ip_filter.Sort();
    std::cout << ip_filter;

    ip_filter.Filter(1);
    std::cout << ip_filter;

    ip_filter.Filter(46, 70);
    std::cout << ip_filter;
    
    ip_filter.FilterAny(46);
    std::cout << ip_filter;

  } catch(const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
