#pragma once

#include <iostream>
#include <type_traits>
#include <tuple>
#include <cstdint>

namespace homework_04 {

/** Brief description which ends at this dot. Details follow
 *  here.
 */
template <typename T, std::enable_if_t<std::is_same_v<T, int8_t>, int> = 0>
void print_ip(T value) {
  auto v = static_cast<uint8_t>(value);
  std::cout << +v << "\n";
}

template <typename T, std::enable_if_t<std::is_same_v<T, int16_t>, int> = 0>
void print_ip(T value) {
  auto v = static_cast<float>(value);
  std::cout << v << "\n";
}

template <typename T, std::enable_if_t<std::is_integral_v<T> && sizeof(T) >= sizeof(int32_t), int> = 0>
void print_ip(T value) {
  union {
    T int_ip;
    struct {
      uint8_t items[sizeof(T)];
    };
  } ip;

  ip.int_ip = value;
  for (int i = sizeof(T) - 1; i >= 0; --i) {
    std::cout << +ip.items[i];
    if (i > 0) {
      std::cout << '.';
    }
  }

  std::cout << "\n";
}


template <typename T, std::enable_if_t<std::is_same_v<T, std::string>, int> = 0>
void print_ip(T value) {
  std::cout << value << "\n";
}

template <
  typename T,
  std::enable_if_t<
    !std::is_same_v<std::string, T> &&
    std::is_same_v<
      decltype(std::declval<T>().begin()),
      decltype(std::declval<T>().end())>
      //&& std::is_same_v<decltype(std::advance(std::declval<T>().begin(), 1)), void>
      ,
    int> = 0>
void print_ip(T value) {
  auto it = value.begin();
  auto end = value.end();
  while(it != end) {
    std::cout << *it;
    std::advance(it, 1);
    if (it != end) {
      std::cout << '.';
    }
  }
  std::cout << '\n';
}

template <typename T, typename... Ts>
struct are_all_types_same {
  using type = T;
  //using next_are_all_types_same = typename are_all_types_same<Ts...>; // MSVC 2019
  using next_are_all_types_same = are_all_types_same<Ts...>; // GCC 11
  static constexpr bool value = next_are_all_types_same::value && std::is_same_v<type, typename next_are_all_types_same::type>;
};

template <typename T1, typename T2>
struct are_all_types_same<T1, T2> {
  using type = T2;
  static constexpr bool value = std::is_same_v<T1, T2>;
};

template <typename> struct is_mono_tuple: std::false_type {};

template <typename ...T> struct is_mono_tuple<std::tuple<T...>> {
  static constexpr bool value = are_all_types_same<T...>::value;
};

template <typename Tuple, int N>
struct out_tuple {
  static void out(const Tuple& t) {
    std::cout << std::get<std::tuple_size_v<Tuple> - N>(t) << '.';
    out_tuple<Tuple, N - 1>::out(t);
  }
};

template <typename Tuple>
struct out_tuple<Tuple, 1> {
  static void out(const Tuple& t) {
    std::cout << std::get<std::tuple_size_v<Tuple> - 1>(t);
  }
};

template <typename T, std::enable_if_t<is_mono_tuple<T>::value, int> = 0>
void print_ip(T value) {
  out_tuple<T, std::tuple_size_v<T>>::out(value);
  std::cout << "\n";
}

}  // namespace homework_04
