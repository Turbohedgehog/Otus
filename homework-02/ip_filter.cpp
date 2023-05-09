#include "ip_filter.h"

#include <iostream>
#include <stdexcept>

namespace homework_02 {

IPFilter::IPFilter()
  : regex_(IPFilter::kLambda.data()) {}

void IPFilter::AddAddress(std::string str_value) {
  std::smatch matches;
  if (!std::regex_search(str_value, matches, regex_)) {
    throw std::runtime_error("Cannot parse expression");
  }

  IPv4 ip;
  for (std::size_t idx = 0; idx < std::tuple_size<IPv4>::value; ++idx) {
    auto num_value = std::stoi(matches[idx + 1]);
    if (num_value < 0 || num_value > 255) {
      throw std::runtime_error("The ip segment value must be between 0 and 255");
    }

    ip[idx] = static_cast<IPv4::value_type>(num_value);
  }

  ip_addresses_.emplace_back(ip);
}

void IPFilter::Sort() {
  std::sort(ip_addresses_.begin(), ip_addresses_.end(), std::greater<>());
}

void IPFilter::ResetFilter() {
  filter_ = std::function<bool(const IPv4&)>();
}

void IPFilter::FilterAny(IPv4::value_type value) {
  filter_ = [=] (const IPv4& ip) -> bool {
    for (std::size_t i = 0; i < ip.size(); ++i) {
      if (ip[i] == value) {
        return true;
      }
    }

    return false;
  };
}

std::ostream& operator<<(std::ostream& out, const IPFilter& ip_filter) {
  for(const auto& ip : ip_filter.ip_addresses_) {
    if (ip_filter.filter_ && !ip_filter.filter_(ip)) {
      continue;
    }

    for (std::size_t idx = 0; idx < ip.size(); ++idx) {
      out << std::to_string(ip[idx]) << ((idx != ip.size() - 1) ? '.' : '\n');
    }
  }

  return out;
}

}  // namespace homework_02
