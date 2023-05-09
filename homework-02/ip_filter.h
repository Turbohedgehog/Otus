#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <regex>
#include <array>
#include <functional>

namespace homework_02 {

using IPv4 = std::array<uint8_t, 4>;

class IPFilter {
 public:
  IPFilter();
  void AddAddress(std::string raw_value) noexcept(false);
  void Sort();
  void ResetFilter();

  template<typename T, typename... Ts>
  void Filter(T&& value, Ts&&... values) { 
    using CT = std::common_type_t<IPv4::value_type, T, Ts...>;
    std::array<CT, sizeof...(Ts) + 1> filter_array {value, std::forward<CT>(values)...};
    filter_ = [=](const IPv4& ip) -> bool { 
      for (std::size_t i = 0; i < ip.size() && i < filter_array.size(); ++i) {
        if (ip[i] != filter_array[i]) {
          return false;
        }
      }

      return true;
    };
  }

  void FilterAny(IPv4::value_type value);

  friend std::ostream& operator<<(std::ostream& out, const IPFilter& ip_filter);

 private:
  std::vector<IPv4> ip_addresses_;
  std::regex regex_;
  std::function<bool(const IPv4&)> filter_;
  constexpr static std::string_view kLambda = "^([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\t([0-9])+\\t([0-9])+$";
};

}  // namespace homework_02
