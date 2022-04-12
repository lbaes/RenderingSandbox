#pragma once
#include <type_traits>

template <typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
constexpr T operator| (T a, T b) {
	return static_cast<T>(static_cast<std::underlying_type_t<T>>(a) | static_cast<std::underlying_type_t<T>>(b));
}